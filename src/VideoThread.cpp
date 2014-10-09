#include "VideoThread.h"

#include <iostream>
#include <sys/syscall.h>

#ifdef __cplusplus
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#endif

OMX_BUFFERHEADERTYPE* VideoThread::_eglBuffer = NULL;
COMPONENT_T* VideoThread::_eglRender = NULL;
void* VideoThread::_eglImage = 0;

VideoThread::VideoThread(const std::string& fileName)
  : _fileName(fileName), _condition(false), _running(false) {
  _vp.setted = false;
  _vp.width = 0;
  _vp.height = 0;
  _vp.fps = 0;
}

VideoThread::~VideoThread() {

}

void VideoThread::start() {
  _thread = boost::thread(&VideoThread::videoDecode, this);
}

bool VideoThread::isRunning() const {
  return _running/* && _thread.timed_join(boost::posix_time::seconds(1))*/;
}

void VideoThread::join() {
  _thread.join();
}

void VideoThread::interrupt() {
  _thread.join();
  _thread.interrupt();
}

void VideoThread::next(void* eglImage) {
  _eglImage = eglImage;
  _condition = true;
  _condition_changed.notify_all();
}

const VideoThread::VideoProperties& VideoThread::getVideoProperties() const {
  return _vp;
}

void VideoThread::videoDecode1() {
  std::cout << "New VideoThread: " << (pid_t)syscall(SYS_gettid) << std::endl;
  _running = true;

  const char* filename = _fileName.c_str();

  OMX_VIDEO_PARAM_PORTFORMATTYPE format;
  OMX_TIME_CONFIG_CLOCKSTATETYPE cstate;
  COMPONENT_T *video_decode = NULL, *video_scheduler = NULL, *clock = NULL;
  COMPONENT_T *list[5];
  TUNNEL_T tunnel[4];
  ILCLIENT_T *client;
  int status = 0;

  memset(list, 0, sizeof(list));
  memset(tunnel, 0, sizeof(tunnel));

  // Open video file ##################################
  AVFormatContext* pFormatCtx = NULL;
  if(avformat_open_input(&pFormatCtx, filename, NULL, NULL) < 0) {
     std::cerr << "File '" << filename << "' could not be opened" << std::endl;
     exit(1);
  }

  if(avformat_find_stream_info(pFormatCtx, NULL) < 0) {
    std::cerr << "'" << filename << "': could not find stream information" << std::endl;
    exit(1);
  }

  // Find the first video stream
  AVCodecContext* pCodecCtx;
  int videoStream = -1;
  for(unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
    if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
      videoStream = i;
      break;
    }
  }

  if(videoStream == -1) {
    std::cerr << "'" << filename << "': could not find a video stream" << std::endl;
    exit(1);
  }

  // Get a pointer to the codec context for the video stream
  pCodecCtx = pFormatCtx->streams[videoStream]->codec;
  _vp.setted = true;
  _vp.width = pCodecCtx->width;
  _vp.height = pCodecCtx->height;
  _vp.fps = pCodecCtx->bit_rate;

  std::cout << _vp.width << "x" << _vp.height << "@" << _vp.fps << std::endl;

  boost::unique_lock<boost::mutex> lock(_stopMutex);
  while(!_condition) {
    _condition_changed.wait(lock);
  }

  // Find the decoder for the video stream
  AVCodec* pCodec;
  pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec == NULL) {
    std::cerr << "'" << filename << "': codec not supported" << std::endl;
    exit(1);
  }

  // Open codec
  if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
    std::cerr << "'" << filename << "': could not open codec" << std::endl;
    exit(1);
  }
  // ##################################################

  if((client = ilclient_init()) == NULL) {
    std::cerr << "Could not init the IL client" << std::endl;
    exit(1);
  }

  if(OMX_Init() != OMX_ErrorNone) {
    ilclient_destroy(client);
    std::cerr << "Could not init the OMX" << std::endl;
    exit(1);
  }

  // Callback
  ilclient_set_fill_buffer_done_callback(client, VideoThread::myFillBufferDone, 0);

  // Create video_decode
  if(ilclient_create_component(client, &video_decode, const_cast<char*>("video_decode"), static_cast<ILCLIENT_CREATE_FLAGS_T>(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_INPUT_BUFFERS)) != 0) {
    status = -14;
  }
  list[0] = video_decode;

  // Create egl_render
  if(status == 0 && ilclient_create_component(client, &_eglRender, const_cast<char*>("egl_render"), static_cast<ILCLIENT_CREATE_FLAGS_T>(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_OUTPUT_BUFFERS)) != 0) {
    status = -14;
  }
  list[1] = _eglRender;

  // Create clock
  if(status == 0 && ilclient_create_component(client, &clock, const_cast<char*>("clock"), static_cast<ILCLIENT_CREATE_FLAGS_T>(ILCLIENT_DISABLE_ALL_PORTS)) != 0) {
    status = -14;
  }
  list[2] = clock;

  memset(&cstate, 0, sizeof(cstate));
  cstate.nSize = sizeof(cstate);
  cstate.nVersion.nVersion = OMX_VERSION;
  cstate.eState = OMX_TIME_ClockStateWaitingForStartTime;
  cstate.nWaitMask = 1;
  if(clock != NULL && OMX_SetParameter(ILC_GET_HANDLE(clock), OMX_IndexConfigTimeClockState, &cstate) != OMX_ErrorNone)
    status = -13;

  // Create video_scheduler
  if(status == 0 && ilclient_create_component(client, &video_scheduler, const_cast<char*>("video_scheduler"), ILCLIENT_DISABLE_ALL_PORTS) != 0) {
    status = -14;
  }
  list[3] = video_scheduler;

  set_tunnel(tunnel, video_decode, 131, video_scheduler, 10);
  set_tunnel(tunnel+1, video_scheduler, 11, _eglRender, 220);
  set_tunnel(tunnel+2, clock, 80, video_scheduler, 12);

  // Setup clock tunnel first
  if(status == 0 && ilclient_setup_tunnel(tunnel+2, 0, 0) != 0) {
    status = -15;
  }
  else {
    ilclient_change_component_state(clock, OMX_StateExecuting);
  }

  if(status == 0) {
    ilclient_change_component_state(video_decode, OMX_StateIdle);
  }

  memset(&format, 0, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
  format.nSize = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
  format.nVersion.nVersion = OMX_VERSION;
  format.nPortIndex = 130;
  format.eCompressionFormat = OMX_VIDEO_CodingAVC;

  if(status == 0 &&
     OMX_SetParameter(ILC_GET_HANDLE(video_decode), OMX_IndexParamVideoPortFormat, &format) == OMX_ErrorNone &&
     ilclient_enable_port_buffers(video_decode, 130, NULL, NULL, NULL) == 0) {
    OMX_BUFFERHEADERTYPE *buf;
    int port_settings_changed = 0;
    int first_packet = 1;
    int pstatus = 0;
    AVPacket packet;
    int index = 0;

    do {
      printf("###before DO!\n");
      pstatus = av_read_frame(pFormatCtx, &packet);

      if(packet.stream_index == videoStream) {
        printf("=>Read frame, status: %d, index: %d, stream index: %d, packet duration: %d, size: %d\n",pstatus,index++,packet.stream_index,packet.duration,packet.size);

        unsigned int psize = packet.size;
        int preaded = 0;

        while(psize != 0) {
          buf = ilclient_get_input_buffer(video_decode, 130, 1);
          unsigned char *dest = buf->pBuffer;

          buf->nFlags = 0;
          buf->nOffset = 0;

          if(first_packet) { buf->nFlags = OMX_BUFFERFLAG_STARTTIME; first_packet = false; }
          else buf->nFlags = OMX_BUFFERFLAG_TIME_UNKNOWN;

          buf->nTimeStamp = packet.duration;
          buf->nFilledLen = (psize > buf->nAllocLen) ? buf->nAllocLen : psize;
          memcpy(dest, packet.data+preaded, buf->nFilledLen);

          psize -= buf->nFilledLen;
          preaded += buf->nFilledLen;

          if(psize == 0) { buf->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME; printf("=>OMX_BUFFERFLAG_ENDOFFRAME\n"); }
          printf("=>BUFF size: %d\n", buf->nFilledLen);

          int nRetry = 0;
          while(true) {
            OMX_ERRORTYPE r = OMX_EmptyThisBuffer(ILC_GET_HANDLE(video_decode), buf);
            if(r == OMX_ErrorNone) {
              break;
            }
            else {
              printf("Failed, OMX_EmptyThisBuffer, error: 0x%08x , buf allocate: %d, buf lenght: %d \n", r, buf->nAllocLen, buf->nFilledLen);
              nRetry++;
            }
            if(nRetry == 5)
              {
                printf("Failed, OMX_EmptyThisBuffer, error: 0x%08x , buf allocate: %d, buf lenght: %d \n", r, buf->nAllocLen, buf->nFilledLen);
                return;
              }
          }

          if(port_settings_changed == 0 &&
             ((preaded > 0 && ilclient_remove_event(video_decode, OMX_EventPortSettingsChanged, 131, 0, 0, 1) == 0) ||
              (preaded == 0 && ilclient_wait_for_event(video_decode, OMX_EventPortSettingsChanged, 131, 0, 0, 1,
                                                       ILCLIENT_EVENT_ERROR | ILCLIENT_PARAMETER_CHANGED, 10000) == 0))) {
            port_settings_changed = 1;
            std::cout << "->Triggered" << std::endl;

            if(ilclient_setup_tunnel(tunnel, 0, 0) != 0) {
              status = -7;
              break;
            }

            ilclient_change_component_state(video_scheduler, OMX_StateExecuting);

            // Now setup tunnel to egl_render
            if(ilclient_setup_tunnel(tunnel+1, 0, 1000) != 0) {
              status = -12;
              break;
            }

            // Set egl_render to idle
            ilclient_change_component_state(_eglRender, OMX_StateIdle);

            // Enable the output port and tell egl_render to use the texture as a buffer
            // ilclient_enable_port(egl_render, 221); THIS BLOCKS SO CANT BE USED
            if(OMX_SendCommand(ILC_GET_HANDLE(_eglRender), OMX_CommandPortEnable, 221, NULL) != OMX_ErrorNone) {
              std::cerr << "OMX_CommandPortEnable failed." << std::endl;
              exit(1);
            }

            if(OMX_UseEGLImage(ILC_GET_HANDLE(_eglRender), &_eglBuffer, 221, NULL, _eglImage) != OMX_ErrorNone) {
              std::cerr << "OMX_UseEGLImage failed." << std::endl;
              exit(1);
            }

            // Set egl_render to executing
            ilclient_change_component_state(_eglRender, OMX_StateExecuting);

            // Request egl_render to write data to the texture buffer
            if(OMX_FillThisBuffer(ILC_GET_HANDLE(_eglRender), _eglBuffer) != OMX_ErrorNone) {
              std::cerr << "OMX_FillThisBuffer failed." << std::endl;
              exit(1);
            }
          }

          ilclient_flush_tunnels(tunnel, 0);
          ilclient_disable_port_buffers(video_decode, 130, NULL, NULL, NULL);
        }//while psize)
        av_free_packet(&packet);
      }//if index
    }  while(pstatus == 0);
  }

  ilclient_disable_tunnel(tunnel);
  ilclient_disable_tunnel(tunnel+1);
  ilclient_disable_tunnel(tunnel+2);
  ilclient_teardown_tunnels(tunnel);

  ilclient_cleanup_components(list);
  ilclient_set_fill_buffer_done_callback(client, NULL, 0);

  ilclient_state_transition(list, OMX_StateIdle);
  ilclient_state_transition(list, OMX_StateLoaded);

  OMX_Deinit();

  ilclient_destroy(client);

  _running = false;

  boost::this_thread::interruption_point();
  std::cout << "VideoThread finished: " << (pid_t)syscall(SYS_gettid) << std::endl;
}

void VideoThread::videoDecode() {
  std::cout << "New VideoThread: " << (pid_t)syscall(SYS_gettid) << std::endl;
  _running = true;

  const char* filename = _fileName.c_str();

  OMX_VIDEO_PARAM_PORTFORMATTYPE format;
  OMX_TIME_CONFIG_CLOCKSTATETYPE cstate;
  COMPONENT_T *video_decode = NULL, *video_scheduler = NULL, *clock = NULL;
  COMPONENT_T *list[5];
  TUNNEL_T tunnel[4];
  ILCLIENT_T *client;
  FILE *in;
  int status = 0;
  unsigned int data_len = 0;

  memset(list, 0, sizeof(list));
  memset(tunnel, 0, sizeof(tunnel));

  if((in = fopen(filename, "rb")) == NULL) {
    std::cerr << "Could not open '" << filename << "'." << std::endl;
    exit(1);
  }

  if((client = ilclient_init()) == NULL) {
    fclose(in);
    std::cerr << "Could not init the IL client" << std::endl;
    exit(1);
  }

  if(OMX_Init() != OMX_ErrorNone) {
    ilclient_destroy(client);
    fclose(in);
    std::cerr << "Could not init the OMX" << std::endl;
    exit(1);
  }

  // Callback
  ilclient_set_fill_buffer_done_callback(client, VideoThread::myFillBufferDone, 0);

  // Create video_decode
  if(ilclient_create_component(client, &video_decode, const_cast<char*>("video_decode"), static_cast<ILCLIENT_CREATE_FLAGS_T>(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_INPUT_BUFFERS)) != 0) {
    status = -14;
  }
  list[0] = video_decode;

  // Create egl_render
  if(status == 0 && ilclient_create_component(client, &_eglRender, const_cast<char*>("egl_render"), static_cast<ILCLIENT_CREATE_FLAGS_T>(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_OUTPUT_BUFFERS)) != 0) {
    status = -14;
  }
  list[1] = _eglRender;

  // Create clock
  if(status == 0 && ilclient_create_component(client, &clock, const_cast<char*>("clock"), static_cast<ILCLIENT_CREATE_FLAGS_T>(ILCLIENT_DISABLE_ALL_PORTS)) != 0) {
    status = -14;
  }
  list[2] = clock;

  memset(&cstate, 0, sizeof(cstate));
  cstate.nSize = sizeof(cstate);
  cstate.nVersion.nVersion = OMX_VERSION;
  cstate.eState = OMX_TIME_ClockStateWaitingForStartTime;
  cstate.nWaitMask = 1;
  if(clock != NULL && OMX_SetParameter(ILC_GET_HANDLE(clock), OMX_IndexConfigTimeClockState, &cstate) != OMX_ErrorNone)
    status = -13;

  // Create video_scheduler
  if(status == 0 && ilclient_create_component(client, &video_scheduler, const_cast<char*>("video_scheduler"), ILCLIENT_DISABLE_ALL_PORTS) != 0) {
    status = -14;
  }
  list[3] = video_scheduler;

  set_tunnel(tunnel, video_decode, 131, video_scheduler, 10);
  set_tunnel(tunnel+1, video_scheduler, 11, _eglRender, 220);
  set_tunnel(tunnel+2, clock, 80, video_scheduler, 12);

  // Setup clock tunnel first
  if(status == 0 && ilclient_setup_tunnel(tunnel+2, 0, 0) != 0) {
    status = -15;
  }
  else {
    ilclient_change_component_state(clock, OMX_StateExecuting);
  }

  if(status == 0) {
    ilclient_change_component_state(video_decode, OMX_StateIdle);
  }

  memset(&format, 0, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
  format.nSize = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
  format.nVersion.nVersion = OMX_VERSION;
  format.nPortIndex = 130;
  format.eCompressionFormat = OMX_VIDEO_CodingAVC;
  format.xFramerate = 25 << 16;

  if(status == 0 &&
     OMX_SetParameter(ILC_GET_HANDLE(video_decode), OMX_IndexParamVideoPortFormat, &format) == OMX_ErrorNone &&
     ilclient_enable_port_buffers(video_decode, 130, NULL, NULL, NULL) == 0) {
    OMX_BUFFERHEADERTYPE *buf;
    int port_settings_changed = 0;
    int first_packet = 1;

    ilclient_change_component_state(video_decode, OMX_StateExecuting);

    fseek(in, 0L, SEEK_END);
    unsigned int fsize = ftell(in);
    fseek(in, 0L, SEEK_SET);
    while((buf = ilclient_get_input_buffer(video_decode, 130, 1)) != NULL) {
      // Feed data and wait until we get port settings changed
      unsigned char *dest = buf->pBuffer;

      // Loop if at end
      if(feof(in)) {
        //fseek(in, 0, SEEK_SET);
        break;
      }

      data_len += fread(dest, 1, buf->nAllocLen-data_len, in);

      if(port_settings_changed == 0 &&
         ((data_len > 0 && ilclient_remove_event(video_decode, OMX_EventPortSettingsChanged, 131, 0, 0, 1) == 0) ||
          (data_len == 0 && ilclient_wait_for_event(video_decode, OMX_EventPortSettingsChanged, 131, 0, 0, 1,
                                                    ILCLIENT_EVENT_ERROR | ILCLIENT_PARAMETER_CHANGED, 10000) == 0))) {
        port_settings_changed = 1;

        if(!_vp.setted) {
          OMX_PARAM_PORTDEFINITIONTYPE param;
          memset(&param, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
          param.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
          param.nVersion.nVersion = OMX_VERSION;
          param.nPortIndex = 131;

          OMX_GetParameter(ILC_GET_HANDLE(video_decode), OMX_IndexParamPortDefinition, &param);

          _vp.setted = true;
          _vp.width = param.format.video.nFrameWidth;
          _vp.height = param.format.video.nFrameHeight;
          _vp.fps = param.format.video.xFramerate;

          boost::unique_lock<boost::mutex> lock(_stopMutex);
          while(!_condition) {
            _condition_changed.wait(lock);
          }
        }

        if(ilclient_setup_tunnel(tunnel, 0, 0) != 0) {
          status = -7;
          break;
        }

        ilclient_change_component_state(video_scheduler, OMX_StateExecuting);

        // Now setup tunnel to egl_render
        if(ilclient_setup_tunnel(tunnel+1, 0, 1000) != 0) {
          status = -12;
          break;
        }

        // Set egl_render to idle
        ilclient_change_component_state(_eglRender, OMX_StateIdle);

        // Enable the output port and tell egl_render to use the texture as a buffer
        // ilclient_enable_port(egl_render, 221); THIS BLOCKS SO CANT BE USED
        if(OMX_SendCommand(ILC_GET_HANDLE(_eglRender), OMX_CommandPortEnable, 221, NULL) != OMX_ErrorNone) {
          std::cerr << "OMX_CommandPortEnable failed." << std::endl;
          exit(1);
        }

        if(OMX_UseEGLImage(ILC_GET_HANDLE(_eglRender), &_eglBuffer, 221, NULL, _eglImage) != OMX_ErrorNone) {
          std::cerr << "OMX_UseEGLImage failed." << std::endl;
          exit(1);
        }

        // Set egl_render to executing
        ilclient_change_component_state(_eglRender, OMX_StateExecuting);

        // Request egl_render to write data to the texture buffer
        if(OMX_FillThisBuffer(ILC_GET_HANDLE(_eglRender), _eglBuffer) != OMX_ErrorNone) {
          std::cerr << "OMX_FillThisBuffer failed." << std::endl;
          exit(1);
        }
      }

      if(!data_len) {
        break;
      }

      buf->nFilledLen = data_len;
      data_len = 0;

      buf->nOffset = 0;
      if(first_packet) {
        buf->nFlags = OMX_BUFFERFLAG_STARTTIME;
        first_packet = 0;
      }
      else {
        buf->nFlags = OMX_BUFFERFLAG_TIME_UNKNOWN;
      }

      if(OMX_EmptyThisBuffer(ILC_GET_HANDLE(video_decode), buf) != OMX_ErrorNone) {
        status = -6;
        break;
      }
    }

    buf->nFilledLen = 0;
    buf->nFlags = OMX_BUFFERFLAG_TIME_UNKNOWN | OMX_BUFFERFLAG_EOS;

    if(OMX_EmptyThisBuffer(ILC_GET_HANDLE(video_decode), buf) != OMX_ErrorNone) {
      status = -20;
    }

    // Need to flush the renderer to allow video_decode to disable its input port
    ilclient_flush_tunnels(tunnel, 0);

    ilclient_disable_port_buffers(video_decode, 130, NULL, NULL, NULL);
  }

  fclose(in);

  ilclient_disable_tunnel(tunnel);
  ilclient_disable_tunnel(tunnel+1);
  ilclient_disable_tunnel(tunnel+2);
  ilclient_teardown_tunnels(tunnel);

  ilclient_cleanup_components(list);
  ilclient_set_fill_buffer_done_callback(client, NULL, 0);

  ilclient_state_transition(list, OMX_StateIdle);
  ilclient_state_transition(list, OMX_StateLoaded);

  OMX_Deinit();

  ilclient_destroy(client);

  _running = false;

  boost::this_thread::interruption_point();
  std::cout << "VideoThread finished: " << (pid_t)syscall(SYS_gettid) << std::endl;
}

void VideoThread::myFillBufferDone(void* data, COMPONENT_T* comp) {
  if(OMX_FillThisBuffer(ilclient_get_handle(_eglRender), _eglBuffer) != OMX_ErrorNone) {
    std::cerr << "OMX_FillThisBuffer failed in callback" << std::endl;
    exit(1);
  }
}
