/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#pragma once

#include <time.h>
#include "hantro_dec/basetype.h"
#include <vpu_wrapper.h>
#include "ivpumft.h"
#include "CVpuMediaBuffer.h"
#include "CSampleQueue.h"
#include <Mfidl.h>

#define MFT_MAX_STREAMS             1
#define MFT_OUTPUT_WIDTH            1920
#define MFT_OUTPUT_HEIGHT           960
// #define MFT_OUTPUT_WIDTH            1280
// #define MFT_OUTPUT_HEIGHT           720
#define MFT_FRAMERATE_NUMERATOR     30
#define MFT_FRAMERATE_DENOMINATOR   1
#define MFT_DEFAULT_SAMPLE_DURATION 400000 // 1/25th of a second in hundred nanoseconds
#define MFT_MILLISECOND_TIMEBASE    10000  // 1 ms in hundred nanoseconds
#define MFT_SECOND_TIMEBASE         10000000  // 1 sec in hundred nanonseconds

// Hantro H265 VPU needs 6 as minimum, VP9 needs 9.  MFT appears to need 6 buffers in flight.
// Set the maximum number of buffers to 18.
#define MFT_MIN_BUFFERS             6  // Minimum number of frame buffers
#define MFT_MAX_BUFFERS             18 // Maximum number of frame buffers

// Helper Macros
#define SAFERELEASE(x) \
    if((x) != NULL) \
    { \
        (x)->Release(); \
        (x) = NULL; \
    } \

#define SAFEDELETE(x) \
    if((x) != NULL) \
    { \
        delete (x); \
        (x) = NULL; \
    } \

enum eMFTStatus
{
    MYMFT_STATUS_INPUT_ACCEPT_DATA      = 0x00000001,   /* The MFT can accept input data */
    MYMFT_STATUS_OUTPUT_SAMPLE_READY    = 0x00000002,
    MYMFT_STATUS_STREAM_STARTED         = 0x00000004,
    MYMFT_STATUS_DRAINING               = 0x00000008,   /* See http://msdn.microsoft.com/en-us/library/dd940418(v=VS.85).aspx
                                                        ** While the MFT is is in this state, it must not send
                                                        ** any more METransformNeedInput events. It should continue
                                                        ** to send METransformHaveOutput events until it is out of
                                                        ** output. At that time, it should send METransformDrainComplete */
};

struct _codecMap
{
    const GUID* guid; 
    const VpuCodStd codec;
};

extern const struct _codecMap g_ppguidInputTypes[];
extern  const   DWORD   g_dwNumInputTypes;
extern  const   GUID*   g_ppguidOutputTypes[];
extern  const   DWORD   g_dwNumOutputTypes;
extern  const   GUID    VpuMft_MFSampleExtension_Marker;

class CVpuMft :
    public IMFTransform,
    public IMFShutdown,
    public IMFMediaEventGenerator,
    public IMFAsyncCallback,
    public IMFQualityAdvise,
    public IVpuMft
    /*****************************
    ** Todo: if you add interfaces
    ** ensure that you add them
    ** to QueryInterface
    *****************************/
{
public:
    static  volatile    ULONG   m_ulNumObjects;                 // Total object count

    // Initializer
    static  HRESULT     CreateInstance(IMFTransform** ppHWMFT);

#pragma region IUnknown
    // IUnknown Implementations
    ULONG   __stdcall   AddRef(void);
    HRESULT __stdcall   QueryInterface(
        REFIID  riid,
        void**  ppvObject
    );
    ULONG   __stdcall   Release(void);
#pragma endregion IUnknown

#pragma region IMFTransform
    // IMFTransform Implementations
    HRESULT __stdcall   AddInputStreams(
        DWORD   dwStreams,
        DWORD*  pdwStreamIDs
    );
    HRESULT __stdcall   DeleteInputStream(
        DWORD   dwStreamID
    );
    HRESULT __stdcall   GetAttributes(
        IMFAttributes** ppAttributes
    );
    HRESULT __stdcall   GetInputAvailableType(
        DWORD           dwInputStreamID,
        DWORD           dwTypeIndex,
        IMFMediaType**  ppType
    );
    HRESULT __stdcall   GetInputCurrentType(
        DWORD           dwInputStreamID,
        IMFMediaType**  ppType
    );
    HRESULT __stdcall   GetInputStatus(
        DWORD   dwInputStreamID,
        DWORD*  pdwFlags
    );
    HRESULT __stdcall   GetInputStreamAttributes(
        DWORD           dwInputStreamID,
        IMFAttributes** ppAttributes
    );
    HRESULT __stdcall   GetInputStreamInfo(
        DWORD                   dwInputStreamID,
        MFT_INPUT_STREAM_INFO*  pStreamInfo
    );
    HRESULT __stdcall   GetOutputAvailableType(
        DWORD           dwOutputStreamID,
        DWORD           dwTypeIndex,
        IMFMediaType**  ppType
    );
    HRESULT __stdcall   GetOutputCurrentType(
        DWORD           dwOutputStreamID,
        IMFMediaType**  ppType
    );
    HRESULT __stdcall   GetOutputStatus(
        DWORD*  pdwFlags
    );
    HRESULT __stdcall   GetOutputStreamAttributes(
        DWORD           dwOutputStreamID,
        IMFAttributes** ppAttributes
    );
    HRESULT __stdcall   GetOutputStreamInfo(
        DWORD                   dwOutputStreamID,
        MFT_OUTPUT_STREAM_INFO* pStreamInfo
    );
    HRESULT __stdcall   GetStreamCount(
        DWORD*  pdwInputStreams,
        DWORD*  pdwOutputStreams
    );
    HRESULT __stdcall   GetStreamIDs(
        DWORD   dwInputIDArraySize,
        DWORD*  pdwInputIDs,
        DWORD   dwOutputIDArraySize,
        DWORD*  pdwOutputIDs
    );
    HRESULT __stdcall   GetStreamLimits(
        DWORD*  pdwInputMinimum,
        DWORD*  pdwInputMaximum,
        DWORD*  pdwOutputMinimum,
        DWORD*  pdwOutputMaximum
    );
    HRESULT __stdcall   ProcessEvent(
        DWORD           dwInputStreamID,
        IMFMediaEvent*  pEvent
    );
    HRESULT __stdcall   ProcessInput(
        DWORD       dwInputStreamID,
        IMFSample*  pSample,
        DWORD       dwFlags
    );
    HRESULT __stdcall   ProcessMessage(
        MFT_MESSAGE_TYPE eMessage,
        ULONG_PTR ulParam
    );
    HRESULT __stdcall   ProcessOutput(
        DWORD                   dwFlags,
        DWORD                   dwOutputBufferCount,
        MFT_OUTPUT_DATA_BUFFER* pOutputSamples,
        DWORD*                  pdwStatus
    );
    HRESULT __stdcall   SetInputType(
        DWORD           dwInputStreamID,
        IMFMediaType*   pType,
        DWORD           dwFlags
    );
    HRESULT __stdcall   SetOutputBounds(
        LONGLONG hnsLowerBound,
        LONGLONG hnsUpperBound
    );
    HRESULT __stdcall   SetOutputType(
        DWORD           dwOutputStreamID,
        IMFMediaType*   pType,
        DWORD           dwFlags
    );
#pragma endregion IMFTransform

#pragma region IMFShutdown
    // IMFShutdown Implementations
    HRESULT __stdcall   GetShutdownStatus(
        MFSHUTDOWN_STATUS*  pStatus
    );
    HRESULT __stdcall   Shutdown(void);
#pragma endregion IMFShutdown

#pragma region IMFMediaEventGenerator
    // IMFMediaEventGenerator Implementations
    HRESULT __stdcall   BeginGetEvent(
        IMFAsyncCallback*   pCallback,
        IUnknown*           punkState
    );
    HRESULT __stdcall   EndGetEvent(
        IMFAsyncResult* pResult,
        IMFMediaEvent** ppEvent
    );
    HRESULT __stdcall   GetEvent(
        DWORD           dwFlags,
        IMFMediaEvent** ppEvent
    );
    HRESULT __stdcall   QueueEvent(
        MediaEventType      met,
        REFGUID             guidExtendedType,
        HRESULT             hrStatus,
        const PROPVARIANT*  pvValue
    );
#pragma endregion IMFMediaEventGenerator

#pragma region IMFAsyncCallback
    // IMFAsyncCallback Implementations
    HRESULT __stdcall   GetParameters(
        DWORD*  pdwFlags,
        DWORD*  pdwQueue
    );
    HRESULT __stdcall   Invoke(
        IMFAsyncResult *pAsyncResult
    );
#pragma endregion IMFAsyncCallback

#pragma region IMFQualityAdvise
    HRESULT STDMETHODCALLTYPE SetDropMode(
            /* [in] */ MF_QUALITY_DROP_MODE eDropMode);

    HRESULT STDMETHODCALLTYPE SetQualityLevel(
            /* [in] */ MF_QUALITY_LEVEL eQualityLevel);

    HRESULT STDMETHODCALLTYPE GetDropMode(
            /* [annotation][out] */
            _Out_  MF_QUALITY_DROP_MODE *peDropMode);

    HRESULT STDMETHODCALLTYPE GetQualityLevel(
            /* [annotation][out] */
            _Out_  MF_QUALITY_LEVEL *peQualityLevel);

    HRESULT STDMETHODCALLTYPE DropTime(
            /* [in] */ LONGLONG hnsAmountToDrop);
#pragma endregion IMFQualityAdvise

    // IVpuMft Implementations
    HRESULT __stdcall   DecodeInputFrame(
        IMFSample*  pInputSample
    );

    HRESULT __stdcall   ScheduleFrameDecode(void);

protected:
    CVpuMft(void);
    ~CVpuMft(void);

    HRESULT             InitializeTransform(void);
    HRESULT             CheckInputType(
        IMFMediaType*   pMT,
        VpuCodStd*      pCodec
    );
    HRESULT             CheckOutputType(
        IMFMediaType*   pMT
    );
    HRESULT             ShutdownEventQueue(void);

    HRESULT             AllocVpuMem();
    HRESULT             FreeVpuMem();
    void *              Align(void *ptr, unsigned int align);
    int                 Align(int i, unsigned int align);

    BOOL                IsKeyFrame(IMFSample* pSample);
    BOOL                IsInterlaced(UINT32 interlaceMode);

    /******* MFT Media Event Handlers**********/
    HRESULT             OnStartOfStream(void);
    HRESULT             OnEndOfStream(void);
    HRESULT             OnDrain(
        const UINT32 un32StreamID
    );
    HRESULT             OnFlush(void);
    HRESULT             OnMarker(
        const ULONG_PTR pulID
    );
    /***********End Event Handlers************/
    HRESULT             RequestSample(
        const UINT32 un32StreamID
    );
    HRESULT             UpdateSampleTime(ULONG ulSampleIdx, IMFSample *pSample);
    HRESULT             FlushSamples(void);
    HRESULT             CreateOutputSample(ULONG ulSampleIdx, IMFSample *pInputSample, IMFSample **pOutputSample);
    HRESULT             SendDrainCompleteEvent(void);
    HRESULT             SendOutputFrameEvent(IMFSample* pOutputSample);
    BOOL                IsLocked(void);
    BOOL                IsMFTReady(void);

    // Member variables
    volatile    ULONG   m_ulRef;
    volatile    ULONG   m_ulInputCounter;
    volatile    ULONG   m_ulOutputCounter;
    volatile    ULONG   m_ulInputSamplesAdded;
    volatile    ULONG   m_ulInputSamplesDequeued;
    volatile    ULONG   m_ulKeyFrames;
    volatile    ULONG   m_ulDroppedFrames;
    volatile LONGLONG   m_llNextSampleTime;
    volatile    ULONG   m_Sub5000;
    volatile    ULONG   m_Above5000;
    volatile    ULONG   m_AboveFPS;

    MF_QUALITY_DROP_MODE m_dropMode;

    static struct _timespec64   m_clockStart;
    struct _timespec64  m_streamStart;
    IMFMediaType*       m_pInputMT;
    IMFMediaType*       m_pOutputMT;
    IMFAttributes*      m_pAttributes;
    MFRatio             m_fps;
    UINT32              m_inputHeight;
    UINT32              m_inputWidth;
    UINT32              m_outputHeight;
    UINT32              m_outputWidth;
    UINT32              m_uiInterlaceMode;
    BOOL                m_bInterlaced;

    IMFMediaEventQueue* m_pEventQueue;
    DWORD               m_dwStatus;
    DWORD               m_dwNeedInputCount;
    DWORD               m_dwHaveOutputCount;
    DWORD               m_dwPendingFrameDecodeCount;
    DWORD               m_dwDecodeWorkQueueID;
    BOOL                m_bFirstSample;
    BOOL                m_bShutdown;
    BOOL                m_bDXVA;
    BOOL                m_bStreamChange;
    CSampleQueue*       m_pInputSampleQueue;
    CSampleQueue*       m_pOutputSampleQueue;
    CRITICAL_SECTION    m_csLock;

    BOOL                m_vpuDecLoaded;
    BOOL                m_vpuDecInit;
    VpuMemInfo          m_vpuMemInfo;
    VpuDecHandle        m_vpuHandle;
    VpuMemDesc          m_vpuMemBlocks[VPU_DEC_MAX_NUM_MEM_REQS];
    VpuMemDesc          m_vpuDescArray[MFT_MAX_BUFFERS];
    VpuFrameBuffer      m_vpuFrameBuffers[MFT_MAX_BUFFERS];
    IVpuBufferCollection* m_bufferCollection;
    VpuDecInitInfo      m_vpuInfo;
    ULONG               m_ulYSize;
    ULONG               m_ulUSize;
    ULONG               m_ulVSize;
};

HRESULT DuplicateAttributes(
                        IMFAttributes*  pDest,
                        IMFAttributes*  pSource
                        );

bool MyFailed(HRESULT hr);
void GetElapsedTime(struct _timespec64 *startTime, struct _timespec64 *elapsedTime);
