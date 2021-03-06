
#ifndef  MN_COMM_H
#define  MN_COMM_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "vos.h"
#include "Ps.h"
#include "pslog.h"
#include "TafTypeDef.h"
#include "TafClientApi.h"
#include "product_config.h"
#include "MnClient.h"

#include "TafOamInterface.h"
#include "NasStkInterface.h"
#include "AppVcApi.h"
#include "TafMntn.h"

#include "NasCcIe.h"

#include "NasSms.h"


/*****************************************************************************
  2 常量定义
*****************************************************************************/

/* LOCAL */
#ifdef  LOCAL
#undef  LOCAL
#endif

#ifdef _EXPORT_LOCAL
#define LOCAL
#else
#define LOCAL static
#endif
enum MMA_MN_MSG_TYPE_ENUM
{

    /* MMA->TAF */
    MMCM_OM_MAINTAIN_INFO_IND                               = 0x00000000,       /* _H2ASN_MsgChoice MNPH_USIM_STATUS_IND_STRU */
    MN_USIM_STATUS_IND                                      = 0x00000001,       /* _H2ASN_MsgChoice MMCM_OM_MAINTAIN_INFO_IND_STRU */
    MMA_TAF_POWER_OFF_IND                                   = 0x00000002,       /* _H2ASN_MsgChoice TAF_MMA_POWER_OFF_IND_STRU */
    ID_MMA_MSG_CS_SERVICE_CHANGE_NOTIFY                     = 0x00000003,       /* _H2ASN_MsgChoice MMA_MSG_CS_SERVICE_IND */
    ID_MMA_TAF_SERVICE_STATUS_CHANGE_NOTIFY                 = 0x00000004,       /* _H2ASN_MsgChoice MMA_TAF_SERVICE_STATUS_CHANGE_NOTIFY_STRU */
    ID_MMA_TAF_NETWORK_CAPABILITY_CHANGE_NOTIFY             = 0x00000005,       /* _H2ASN_MsgChoice MMA_TAF_NETWORK_CAPABILITY_CHANGE_NOTIFY_STRU */
    ID_MMA_TAF_RAT_CHANGE_NOTIFY                            = 0x00000006,       /* _H2ASN_MsgChoice MMA_TAF_RAT_CHANGE_NOTIFY_STRU */

    ID_MMA_TAF_IMS_VOICE_CAP_IND                            = 0x00000007,       /* _H2ASN_MsgChoice MMA_TAF_IMS_VOICE_CAP_IND_STRU */



    /* MSG<->SPM */
    ID_TAF_SPM_SMMA_IND                                     = ID_TAF_MSG_INTERNAL_BASE,       /* _H2ASN_MsgChoice TAF_SPM_SMMA_IND_STRU */
    ID_TAF_SPM_MSG_REPORT_IND                               ,                                 /* _H2ASN_MsgChoice TAF_SPM_MSG_REPORT_IND_STRU */
    ID_TAF_SPM_SMMA_RSP                                     ,                                 /* _H2ASN_MsgChoice TAF_SPM_SMMA_RSP_STRU */

    ID_TAF_SPM_MSG_CHECK_RESULT_IND,                                            /* _H2ASN_MsgChoice TAF_SPM_MSG_CHECK_RESULT_IND_STRU */
    /* 最后一条消息 */
    ID_MMA_MSG_TYPE_BUTT
};
typedef VOS_UINT32  MMA_MN_MSG_TYPE_ENUM_UINT32;


/*****************************************************************************
 枚举名    : TAF_FDN_ME_STATUS_ENUM
 结构说明  : ME是否启动FDN业务检查
             0: TAF_FDN_CHECK_FEATURE_OFF  ME不启动FDN检查
             1: TAF_FDN_CHECK_FEATURE_ON   ME启动FDN检查
*****************************************************************************/
enum TAF_FDN_ME_STATUS_ENUM
{
    TAF_FDN_CHECK_FEATURE_OFF,
    TAF_FDN_CHECK_FEATURE_ON,
    TAF_FDN_CHECK_FEATURE_BUTT
};
typedef VOS_UINT32 TAF_FDN_ME_STATUS_ENUM_UINT32;

/*****************************************************************************
 枚举名    : TAF_FDN_FEATURE_STATUS_ENUM
 结构说明  : ME不支持FDN功能时是否参照协议禁止所有呼叫
             0: TAF_FDN_FEATURE_DISABLE  FDN功能
             1: TAF_FDN_FEATURE_ENABLE   FDN功能禁止所有呼叫
*****************************************************************************/
enum TAF_FDN_FEATURE_STATUS_ENUM
{
    TAF_FDN_FEATURE_DISABLE,
    TAF_FDN_FEATURE_ENABLE,
    TAF_FDN_FEATURE_BUTT
};
typedef VOS_UINT32 TAF_FDN_FEATURE_STATUS_ENUM_UINT32;


/* 定时器名称分类 */
enum
{
    MN_TIMER_CLASS_CCA                                      = 0,
    MN_TIMER_CLASS_MSG                                      = 0x00001000,
    MN_TIMER_CLASS_SSA                                      = 0x00002000,
    MN_TIMER_CLASS_MMA                                      = 0x00004000,
    MN_TIMER_CLASS_APS                                      = 0x00008000,
    MN_TIMER_CLASS_SPM                                      = 0x0000C000,
    MN_TIMER_CLASS_MASK                                     = 0xfffff000
};

#define MN_MO_CTRL_LAI_LEN                                  (9)

/* 定义TAF_FID优先级 */
#define TAF_TASK_PRIORITY                                   (145)

/* 表示所有的Client */
#define MN_CLIENT_ALL                                       ((MN_CLIENT_ID_T)(-1))
#define MN_OP_ID_BUTT                                       ((MN_OPERATION_ID_T)(-1))


#define MN_COM_SRVDOMAIN_CS                                 (1)
#define MN_COM_SRVDOMAIN_PS                                 (2)

/* MN_USIM_STATUS_IND和MMCM_OM_MAINTAIN_INFO_IND放在了MMA_MN_MSG_TYPE_ENUM_UINT32中 */

#define MN_WORD_UCS2_ENCODE_LEN                             (2)


/*当前网络类型*/
#define MN_NET_MODE_INVALID                                 (3)                 /* 网络模式无效 */
#define MN_NET_MODE_LTE                                     (2)                 /* L模式 */
#define MN_NET_MODE_WCDMA                                   (1)                 /* W模式 */
#define MN_NET_MODE_GSM                                     (0)                 /* G模式 */

#define MN_MAX_GSM7BITDEFALPHA_NUM                          (128)

#define NAS_EVT_TYPE_DEF( usSendPid, usMsgId )\
        ((VOS_UINT32)((( usSendPid << 16 ) & ( 0xFFFF0000 ))\
                      |(( usMsgId ) & ( 0x0000FFFF ))))

typedef struct
{
    VOS_MSG_HEADER
    unsigned long                       ulMsgName;

    VOS_UINT8                           ucOmConnectFlg;                         /* UE与PC工具的连接标志, VOS_TRUE: 已连接; VOS_FALSE: 未连接 */
    VOS_UINT8                           ucOmPcRecurEnableFlg;                   /* OM配置的发送NAS PC回放消息的使能标志 */

    VOS_UINT8                           aucRsv2[2];                             /* 保留   */
}MMCM_OM_MAINTAIN_INFO_IND_STRU;



/* begin V7R1 PhaseI Modify */
/*****************************************************************************
 枚举名    : MNPH_USIM_STATUS_ENUM
 结构说明  : MNPH_USIM_STATUS_ENUM消息中的 enUsimStatus
*****************************************************************************/
enum MNPH_USIM_STATUS_ENUM
{
    MNPH_USIM_STATUS_AVILABLE         = 0, /* 卡有效 */
    MNPH_USIM_STATUS_NOT_AVAILABLE    = 1, /* 卡无效 */
    MNPH_USIM_STATUS_BUTT
};
typedef VOS_UINT32 MNPH_USIM_STATUS_ENUM_U32;

typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;
    MNPH_USIM_STATUS_ENUM_U32           enUsimStatus;
}MNPH_USIM_STATUS_IND_STRU;
/* end V7R1 PhaseI Modify */

typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;
    VOS_UINT8                           aucReserve[4];
}TAF_MMA_POWER_OFF_IND_STRU;

#define MN_AT_INTERFACE_MSG_HEADER_LEN                      ((sizeof(MN_AT_IND_EVT_STRU) - VOS_MSG_HEAD_LENGTH) - 4)
enum TAF_SERVICE_TYPE_ENUM
{
    TAF_SERVICE_TYPE_CALL           = 0,  /*call 类型 */
    TAF_SERVICE_TYPE_MSG            = 1,  /*MSG  类型 */
    TAF_SERVICE_TYPE_UNASSIGNED     = 2,  /*FDN INFO 开机主动上报消息 */
    TAF_SERVICE_TYPE_BUTT
};
typedef VOS_UINT32  TAF_SERVICE_TYPE_ENUM_U32;

/*****************************************************************************
 枚举名    : TAF_EMERGENCY_NOTIFY_ENUM
 结构说明  :
             0: TAF_EMERGENCY_NOTIFY_DISABLE  紧急呼叫与温保交互NV项关闭
             1: TAF_EMERGENCY_NOTIFY_ENABLE   紧急呼叫与温保交互NV项开启
*****************************************************************************/
enum TAF_EMERGENCY_NOTIFY_ENUM
{
    TAF_EMERGENCY_NOTIFY_DISABLE,
    TAF_EMERGENCY_NOTIFY_ENABLE,
    TAF_EMERGENCY_NOTIFY_BUTT
};
typedef VOS_UINT32 TAF_EMERGENCY_NOTIFY_ENUM_UINT8;

/*****************************************************************************
 枚举名    : TAF_SMS_MO_TYPE_ENUM_UINT8
 结构说明  :
             0: TAF_SMS_MO_TYPE_MO          AT发起的短信
             1: TAF_SMS_MO_TYPE_SMMA        SMMA
             2: TAF_SMS_MO_TYPE_BUFFER_STK  STK发起的短信
*****************************************************************************/
enum TAF_SMS_MO_TYPE_ENUM
{
    TAF_SMS_MO_TYPE_MO,
    TAF_SMS_MO_TYPE_SMMA,
    TAF_SMS_MO_TYPE_BUFFER_STK,
    TAF_SMS_MO_TYPE_MAX
};
typedef VOS_UINT8 TAF_SMS_MO_TYPE_ENUM_UINT8;

/*****************************************************************************
  3 宏定义
*****************************************************************************/
#define TAF_NUMTYE_INTERNAL_NUMBER                          (0x91)

extern VOS_UINT16 f_ausMnAsciiSfxDefAlpha[MN_MAX_GSM7BITDEFALPHA_NUM];


/* 调试输出 */
#define MN_INFO_LOG(str)                    PS_LOG(WUEPS_PID_TAF, 0, PS_PRINT_INFO, str)
#define MN_NORM_LOG(str)                    PS_LOG(WUEPS_PID_TAF, 0, PS_PRINT_NORMAL, str)
#define MN_WARN_LOG(str)                    PS_LOG(WUEPS_PID_TAF, 0, PS_PRINT_WARNING, str)
#define MN_ERR_LOG(str)                     PS_LOG(WUEPS_PID_TAF, 0, PS_PRINT_ERROR, str)

#define MN_INFO_LOG1(str, x1)               PS_LOG1(WUEPS_PID_TAF, 0, PS_PRINT_INFO, str, x1)
#define MN_INFO_LOG2(str, x1, x2)           PS_LOG2(WUEPS_PID_TAF, 0, PS_PRINT_INFO, str, x1, x2)
#define MN_INFO_LOG3(str, x1, x2, x3)       PS_LOG3(WUEPS_PID_TAF, 0, PS_PRINT_INFO, str, x1, x2, x3)
#define MN_INFO_LOG4(str, x1, x2, x3, x4)   PS_LOG4(WUEPS_PID_TAF, 0, PS_PRINT_INFO, str, x1, x2, x3, x4)

#define MN_NORM_LOG1(str, x)                PS_LOG1(WUEPS_PID_TAF, 0, PS_PRINT_NORMAL, str, x)
#define MN_WARN_LOG1(str, x)                PS_LOG1(WUEPS_PID_TAF, 0, PS_PRINT_WARNING, str, x)
#define MN_WARN_LOG2(str, x1,x2)            PS_LOG2(WUEPS_PID_TAF, 0, PS_PRINT_WARNING, str,x1,x2)
#define MN_ERR_LOG1(str, x)                 PS_LOG1(WUEPS_PID_TAF, 0, PS_PRINT_ERROR, str, x)


/* ASSERT */
#ifdef  _DEBUG
#define  MN_ASSERT(expr) \
    if(!(expr)) \
    { \
        MN_ERR_LOG("Assertion failed: " #expr); \
    }
#else
#define  MN_ASSERT(expr)  ((VOS_VOID)0)
#endif /* _DEBUG */

#define MN_GET_TON(enNumType, ucAddrType) ((enNumType) = ((ucAddrType >> 4) & 0x07))
#define MN_GET_NPI(enNumPlan, ucAddrType) ((enNumPlan) = (ucAddrType & 0x0f))
#define MN_GET_ADDRTYPE(ucAddrType, enNumType, enNumPlan)                           \
                        ((ucAddrType) = 0x80 | (VOS_UINT8)((enNumType) << 4) | enNumPlan)

#if !defined(MN_ARRAY_SIZE)
#define MN_ARRAY_SIZE(a)                (sizeof((a)) / sizeof((a[0])))
#endif
#define TAF_CS_GET_CC_ERR_CODE_MAP_TBL_PTR()   (g_astMnCallErrCodeMapTbl)
#define TAF_CS_GET_CC_ERR_CODE_MAP_TBL_SIZE()  (sizeof(g_astMnCallErrCodeMapTbl)/sizeof(TAF_CS_ERR_CODE_MAP_STRU))
/*****************************************************************************
  4 结构定义
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulStatus;                               /*是否激活，0不激活，1激活 */
    VOS_UINT32                          ulCustomizeService;                     /*终端说明书是1个byte，为了没有空洞，扩充成4byte，高3byte保留*/
}MN_NV_ITEM_CUSTOMIZE_SERV_STRU;

/*SMA定时器ID的定义*/
enum MN_MSG_TIMER_ID_ENUM
{
    MN_MSG_TID_WAIT_GET_EF_RSP                             = MN_TIMER_CLASS_MSG,/*初始化,读取USIM中EFSMSS,EFSMSP,EFSMSR,EFSMS文件启动的定时器*/
    MN_MSG_TID_LINK_CTRL,                                                       /*初始化,读取USIM中EFSMSS,EFSMSP,EFSMSR,EFSMS文件启动的定时器*/
    MN_MSG_TID_WAIT_USIM,
    MN_MSG_ID_WAIT_RETRY_PERIOD,                                                /* 短信重发时间周期 */
    MN_MSG_ID_WAIT_RETRY_INTERVAL,                                              /* 短信重发时间间隔 */
    MN_MGS_ID_WAIT_FDN_CHECK,                                                   /* 等待FDN检查结果 */
    MN_MGS_ID_WAIT_MO_SMS_CTRL_CHECK,                                           /* 等待MO SMS CONTROL检查结果 */
#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))
    TI_TAF_CBA_WAIT_GET_CB_EF_RSP,
#if (FEATURE_ON == FEATURE_ETWS)
    TI_TAF_CBA_RCV_ETWS_TEMP_ENABLE_CBS,                                        /* CBS未开启时，收到主通知时临时使能CBS的Timer */
#endif
#endif
    MN_MSG_TID_MAX
};
typedef VOS_UINT32 MN_MSG_TIMER_ID_ENUM_U32;

/* 来自RABM的请求消息结构 */
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          usMsgName;                              /* 消息名 */
    VOS_UINT8                           aucReserve1[2];                             /* 保留 */
} MN_RABM_IND_MSG_STRU;

/*应用查询参数:f_astMnQryMsgParaAttr*/
typedef struct
{
    TAF_PARA_TYPE                       QueryType;
    TAF_UINT8                           aucReserve1[3];
    TAF_UINT32                          ulFixedPartLen;
}MN_QRYMSGPARA_ATTR;

typedef struct{
    TAF_PARA_TYPE                       QueryType;
    TAF_UINT8                           aucReserve1[3];
    TAF_UINT32                          ulVarPartLen;
}MN_QRYMSGVARLEN_UNIT_STRU;


typedef struct
{
    TAF_FDN_ME_STATUS_ENUM_UINT32       enMeStatus;
}TAF_FDN_CONFIG_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
    TAF_CS_SERVICE_ENUM_UINT32          enCSState;
}MMA_MSG_CS_SERVICE_IND;

typedef VOS_VOID (*TAF_STK_MSG_FUNC_MAP)(struct MsgCB * pstMsg);


typedef struct
{
    VOS_UINT32                          ulMsgName;
    VOS_UINT32                          ulAppMsgName;
    TAF_STK_MSG_FUNC_MAP                pStkMsgFunc;
}MN_STK_MSG_FUNC_MAP_STRU;



typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
}MMA_TAF_SERVICE_STATUS_CHANGE_NOTIFY_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
}MMA_TAF_NETWORK_CAPABILITY_CHANGE_NOTIFY_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
}MMA_TAF_RAT_CHANGE_NOTIFY_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;
    VOS_UINT8                           ucImsVoiceAvail;                        /* IMS VOICE是否可用:VOS_TRUE:IMS voice可用; VOS_FALSE: IMS VOICE不可用 */
    VOS_UINT8                           aucReserve[3];                         /* 保留 */
}MMA_TAF_IMS_VOICE_CAP_IND_STRU;



typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    MN_CLIENT_ID_T                      clientId;                               /* Client ID */
    MN_OPERATION_ID_T                   opId;                                   /* Operation ID */
    VOS_UINT8                           aucReserve[1];                         /* 保留 */
    VOS_UINT8                           aucContent[4];                          /* 消息内容 */
}TAF_SPM_SMMA_IND_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    MN_CLIENT_ID_T                      usClientId;                            /* Client ID */
    MN_OPERATION_ID_T                   ucOpId;                                /* Operation ID */
    TAF_SMS_MO_TYPE_ENUM_UINT8          enSmsMoType;                           /* 短信MO type */
    VOS_UINT8                           ucReportFlag;                          /* SMMA report标志 */
    MN_OPERATION_ID_T                   ucBcOpId;                              /* 广播OPID */
    VOS_UINT8                           aucReserve[2];                         /* 保留位 */
    TAF_MSG_SIGNALLING_TYPE_ENUM_UINT32 enSignallingType;                      /* report消息信令类型 */
    MN_MSG_SUBMIT_RPT_EVT_INFO_STRU     stRptEvtInfo;                          /* 短信report事件信息 */
}TAF_SPM_MSG_REPORT_IND_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    MN_CLIENT_ID_T                      clientId;                               /* Client ID */
    MN_OPERATION_ID_T                   opId;                                   /* Operation ID */
    VOS_UINT8                           aucReserve[1];                         /* 保留 */
    TAF_MSG_SIGNALLING_TYPE_ENUM_UINT32 enMsgSignallingType;
}TAF_SPM_SMMA_RSP_STRU;
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    TAF_MSG_ERROR_ENUM_UINT32           enRslt;                                 /* 检查结果 */
    MN_CLIENT_ID_T                      usClientId;                             /* Client ID */
    MN_OPERATION_ID_T                   ucOpId;                                 /* Operation ID */
    VOS_UINT8                           aucReserve[1];                          /* 保留位 */
}TAF_SPM_MSG_CHECK_RESULT_IND_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    MN_CLIENT_ID_T                      clientId;                               /* Client ID */
    MN_OPERATION_ID_T                   opId;                                   /* Operation ID */
    VOS_UINT8                           aucReserve[1];                         /* 保留 */
    VOS_UINT8                           aucContent[4];                          /* 消息内容 */
}TAF_INTERNAL_MSG_STRU;

/* ASN解析结构 */
typedef struct
{
    VOS_UINT32                          ulMsgId;                                /*_H2ASN_MsgChoice_Export MMA_MN_MSG_TYPE_ENUM_UINT32 */
    VOS_UINT8                           aucMsg[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          MMA_MN_MSG_TYPE_ENUM_UINT32
    ****************************************************************************/
}TAF_COMM_MSG_DATA;
/*_H2ASN_Length UINT32*/


typedef struct
{
    VOS_MSG_HEADER
    TAF_COMM_MSG_DATA                   stMsgData;
} TafComm_MSG;


/*****************************************************************************
  5 函数声明
*****************************************************************************/
/* 检查USIM是否可用 */
VOS_BOOL  MN_IsUsimPresent(VOS_VOID);

/* 设置USIM的可用状态 */
VOS_VOID  MN_SetUsimPresent(
    VOS_BOOL                            bPresent
);

/* 向指定的Client发送异步请求的响应 */
VOS_VOID  MN_SendClientResponse(
    MN_CLIENT_ID_T                      clientId,
    MN_OPERATION_ID_T                   opId,
    VOS_UINT32                          ulErrorCode
);

/* 向指定的或所有的Client上报事件 */
VOS_VOID  MN_SendClientEvent(
    MN_CLIENT_ID_T                      clientId,
    MN_CALLBACK_TYPE_T                  callbackType,
    VOS_UINT32                          ulEventType,
    VOS_VOID                            *pEventContent
);


/* Deleted MN_UnPack7Bit, MN_Pack7Bit */



/*号码类型检查,参考协议23040 9.1.2.5*/
TAF_UINT32 MN_ChkNumType(
    MN_MSG_TON_ENUM_U8                  enNumType
);

/*拨号计划类型检查,参考协议23040 9.1.2.5*/
TAF_UINT32 MN_ChkNumPlan(
    MN_MSG_NPI_ENUM_U8                  enNumPlan
);

/*设置指定索引节点的位图*/
VOS_VOID MN_SetBitMap(
    VOS_UINT32                          *pulBitMap,
    VOS_UINT32                          ulIndex,
    VOS_BOOL                            bReset
);

/*获取指定索引节点的位图*/
VOS_UINT32 MN_GetBitMap(
    VOS_UINT32                          *pulBitMap,
    VOS_UINT32                          ulIndex
);

extern VOS_INT8 NAS_Common_Get_Supported_3GPP_Version(VOS_UINT32 ulCnDomainId);

/*比较ASCII字符串和UCS2字符串,比较结果作为输出参数输出
  此函数仅作UCS2高位为0情况的比较: 比较UCS2的低位和ASCII码*/
VOS_VOID MN_CmpAsciiStrAndUcs2StrCaseInsensitive(
    VOS_UINT8                           *pucUcs2Str,
    VOS_UINT32                          ulUcs2Strlen,
    VOS_UINT8                           *pucAsciiStr,
    VOS_BOOL                            *pbEqualFlag
);

VOS_UINT32 MN_DeciDigit2Ascii(
    VOS_UINT8                           aucDeciDigit[],
    VOS_UINT32                          ulLength,
    VOS_UINT8                           aucAscii[]
);

/* 获取当前发送短信时,短信的实际发送域 */
VOS_VOID MN_MSG_GetCurSendDomain(
    MN_MSG_SEND_DOMAIN_ENUM_U8          enHopeSendDomain,
    MN_MSG_SEND_DOMAIN_ENUM_U8          *penRealSendDomain
);

/* 短信发送相关信息导出到异常事件记录 */
VOS_VOID MN_MSG_OutputSmsMoFailureInfo(
    TAF_MSG_ERROR_ENUM_UINT32           enErrorCode,
    NAS_MNTN_SMS_MO_INFO_STRU          *pstSmsMo
);

/* 记录短信发送失败事件 */
VOS_VOID MN_MNTN_RecordSmsMoFailure(TAF_MSG_ERROR_ENUM_UINT32 enErrorCode);
VOS_UINT32 MN_GetFileSize(
    FILE                                *fp,
    VOS_UINT32                          *pulFileSize
);

/* 记录短信ME操作失败事件 */
VOS_VOID MN_MNTN_RecordSmsMeError (
    MNTN_ME_OPERATION_ENUM_UINT32       enOperation,
    VOS_UINT32                          ulErrorNo
);

VOS_VOID MN_CS_SendMsgToOam(
    MN_CLIENT_ID_T                      clientId,
    MN_CALL_EVENT_ENUM_U32              enEventType,
    VOS_UINT32                          ulOamRcvPid,
    MN_CALL_INFO_STRU                  *pstEvent
);

VOS_VOID MN_CS_SendMsg(
    MN_CLIENT_ID_T                      usClientId,
    MN_CALL_EVENT_ENUM_U32              enEventType,
    MN_CALL_INFO_STRU                  *pstEvent
);

VOS_VOID MN_MSG_SendMsg(
    MN_CLIENT_ID_T                      usClientId,
    MN_CALL_EVENT_ENUM_U32              enEventType,
    MN_MSG_EVENT_INFO_STRU             *pstEvent
);


VOS_VOID  MN_SendReportMsg(TAF_UINT8 ucType,TAF_UINT8* pData,TAF_UINT16 usLen);

VOS_VOID MN_SS_SendMsg(
    MN_CLIENT_ID_T                      usClientId,
    VOS_UINT32                          ulEventType,
    TAF_SS_CALL_INDEPENDENT_EVENT_STRU *pstEvent
);


VOS_VOID MN_CMDCNF_SendMsg(
    MN_CLIENT_ID_T                      usClientId,
    MN_OPERATION_ID_T                   ucOpId,
    TAF_UINT32                          ulErrorCode
);

MN_CLIENT_ID_TYPE_UINT16 MN_GetClientIdType(MN_CLIENT_ID_T usClientId);

VOS_UINT32 MN_GetPidFromClientId(MN_CLIENT_ID_T usClientId);

VOS_VOID MN_PH_SendMsg(MN_CLIENT_ID_T usClientId,VOS_UINT8 *pEvent,VOS_UINT32 ulLen);

VOS_VOID MN_DATASTATUS_SendMsg(
    TAF_UINT16                          usClientId,
    TAF_UINT8                           ucDomain,
    TAF_UINT8                           ucRabId,
    TAF_UINT8                           ucStatus,
    TAF_UINT8                           ucCause
);

VOS_VOID MN_SETPARA_SendMsg(
    TAF_UINT16                          usClientId,
    TAF_UINT8                           ucOpId,
    TAF_PARA_SET_RESULT                 ucResult,
    TAF_PARA_TYPE                       ucParaType,
    TAF_VOID                           *pPara
);

VOS_VOID MN_QRYPARA_SendMsg(
    TAF_UINT16                          usClientId,
    TAF_UINT8                           ucOpId,
    TAF_PARA_TYPE                       ucQueryType,
    TAF_UINT16                          usErrorCode,
    TAF_VOID                           *pPara
);

VOS_VOID MN_QRYPARA_SendMsgToAt  (
    TAF_UINT16                          usClientId,
    TAF_UINT8                           OpId,
    TAF_PARA_TYPE                       QueryType,
    TAF_UINT16                          usErrorCode,
    TAF_VOID                           *pPara
);

VOS_VOID MN_VC_SendMsgToAt(
    MN_CLIENT_ID_T                      clientId,
    VOS_UINT32                          ulEventType,
    APP_VC_EVENT_INFO_STRU             *pstEvent
);
VOS_VOID MN_PH_SendMsgToAt(VOS_UINT8  *pEvent,VOS_UINT32 ulLen);
#if ( FEATURE_ON == FEATURE_LTE )
VOS_UINT32 MN_APS_TransCsqReqToL4a(
    VOS_UINT16                      ucClientId,
    VOS_INT8                        ucOpId
);
#endif

VOS_VOID MN_VC_SendMsg(
    MN_CLIENT_ID_T                      usClientId,
    VOS_UINT32                          ulEventType,
    APP_VC_EVENT_INFO_STRU             *pstEvent
);

extern VOS_INT32* MN_MMA_GetBandInfo(VOS_VOID);


VOS_VOID MN_MSG_ProcAppSend(struct MsgCB * pstMsgSend);

VOS_VOID TAF_ProcStkEnvelopeCnf(struct MsgCB * pstMsg);
VOS_VOID  MN_SndAppMsgToSs (struct MsgCB *pstMsg);
#if (VOS_WIN32 == VOS_OS_VER)
VOS_VOID  MN_ProcUsimMsg (struct MsgCB * pstMsg);
#endif




TAF_CS_CAUSE_ENUM_UINT32 TAF_CALL_MapCcCauseToCsCause(
    MN_CALL_CC_CAUSE_ENUM_U8            enCcCause
);

/* Delete MN_GetLaiForMoCtrl */

MN_CLIENT_ID_T MN_GetRealClientId(
    MN_CLIENT_ID_T                      usClientId,
    VOS_UINT32                          ulPid
);

VOS_VOID MN_CS_SendMsgToAt(
    MN_CLIENT_ID_T                      clientId,
    MN_CALL_EVENT_ENUM_U32              enEventType,
    MN_CALL_INFO_STRU                  *pstEvent
);

VOS_VOID MN_MSG_SendMsgToAt(
    MN_CLIENT_ID_T                      clientId,
    MN_MSG_EVENT_ENUM_U32               enEventType,
    MN_MSG_EVENT_INFO_STRU             *pstEvent
);

VOS_VOID MN_SS_SendMsgToAt(
    MN_CLIENT_ID_T                      clientId,
    VOS_UINT32                          ulEventType,
    TAF_SS_CALL_INDEPENDENT_EVENT_STRU *pstEvent
);

VOS_VOID MN_CMDCNF_SendMsgToAt(
    MN_CLIENT_ID_T                      clientId,
    MN_OPERATION_ID_T                   opId,
    TAF_UINT32                          ulErrorCode
);

VOS_VOID MN_DATASTATUS_SendMsgToAt(
    TAF_UINT16                          ClientId,
    TAF_UINT8                           ucDomain,
    TAF_UINT8                           ucRabId,
    TAF_UINT8                           ucStatus,
    TAF_UINT8                           ucCause
);

VOS_VOID MN_SETPARA_SendMsgToAt(
    TAF_UINT16                          ClientId,
    TAF_UINT8                           OpId,
    TAF_PARA_SET_RESULT                 Result,
    TAF_PARA_TYPE                       ParaType,
    TAF_VOID                           *pPara
);
VOS_UINT32 TAF_CALL_IsEventNeedRpt(
    MN_CALL_EVENT_ENUM_U32              enEventType
);
VOS_UINT32 TAF_MSG_IsEventNeedRpt(
    MN_MSG_EVENT_ENUM_U32              enEventType
);
VOS_UINT32 TAF_SS_IsEventNeedRpt (
    TAF_SS_EVENT  ulEvent
);


VOS_VOID MN_CS_ProcEmergencyCallEvent(
    MN_CALL_EVENT_ENUM_U32              enEventType,
    MN_CALL_INFO_STRU                  *pstEvent
);

VOS_VOID MN_SendEmergencyCallStatusToOam(
    TAF_OAM_EMERGENCY_CALL_STATUS_ENUM_UINT8                enEventType
);

VOS_VOID TAF_CALL_SendMsg(
    MN_CLIENT_ID_T                      usClientId,
    MN_CALL_EVENT_ENUM_U32              enEventType,
    VOS_VOID                           *pData,
    VOS_UINT32                          ulLength
);

VOS_UINT32 TAF_GetSpecificFileListRefreshFileInfo(
    VOS_UINT16                          usFileId,
    PS_USIM_REFRESH_IND_STRU           *pstRefreshInd,
    USIMM_APP_TYPE_ENUM_UINT32         *penAppType
);

#if  ( FEATURE_MULTI_MODEM == FEATURE_ON )
VOS_VOID TAF_SendMtcCsSrvInfoInd(VOS_VOID);
#endif

/*****************************************************************************
 函 数 名  : MN_CALL_GetDataCfgInfoFromBc
 功能描述  : 从BC中获取数据业务配置参数，由于API下发的speed参数和上报的不同，
             需要对speed进行转换
 输入参数  : pstSsCompo - 来自网络的Facility IE中的SS component
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2007年9月20日
    作    者   : 丁庆 49431
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID MN_CALL_GetDataCfgInfoFromBc(
    const NAS_CC_IE_BC_STRU             *pstBc,
    MN_CALL_CS_DATA_CFG_INFO_STRU       *pstDataCfgInfo
);



VOS_UINT32  MN_CALL_JudgeMtCallType(
    const NAS_CC_IE_BC_OCTET3_STRU      *pstOctet3,
    const NAS_CC_IE_BC_OCTET5A_STRU     *pstOctet5a,
    MN_CALL_TYPE_ENUM_U8                *penCallType
);


#if (FEATURE_ON == FEATURE_IMS)
VOS_VOID TAF_SndMmaImsSrvInfoNotify(
    VOS_UINT8                           ucImsCallFlg
);
#endif
#if (FEATURE_ON == FEATURE_PTM)
VOS_VOID TAF_SndAcpuOmFaultErrLogInd(
    VOS_VOID                           *pData,
    VOS_UINT32                          ulDataLen
);
#endif

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* TAF_COMM_H */


