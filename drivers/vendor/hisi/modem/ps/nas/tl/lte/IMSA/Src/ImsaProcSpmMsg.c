/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaProcSpmMsg.c
  Description     : 该C文件实现Spm消息处理和Spm消息发送
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaEntity.h"
#include "ImsaPublic.h"
#include "ImsaCallManagement.h"
#include "ImsaProcSpmMsg.h"
#include "ImsaImsAdaption.h"

#include "ImsaProcImsCallMsg.h"

/* xiongxianghui00253310 for PC-LINT 20140210 begin */
#include "ImsaProcUssdMsg.h"
/* xiongxianghui00253310 for PC-LINT 20140210 end */


/*lint -e767*/
#define    THIS_FILE_ID   PS_FILE_ID_IMSAPROCSPMMSG_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/

extern VOS_UINT32 IMSA_CallSendSpmClprCnf
(
    const IMSA_SPM_CALL_GET_CLPR_CNF_STRU  *pstMsg
);

IMSA_SPM_CALL_MSG_ACT_STRU      g_stImsaSpmCallMsgActTbl[]=
{
    {ID_SPM_IMSA_CALL_ORIG_REQ,                     IMSA_CallProcSpmMsgOrig},
    {ID_SPM_IMSA_CALL_SUPS_CMD_REQ,                 IMSA_CallProcSpmMsgSups},
    {ID_SPM_IMSA_CALL_START_DTMF_REQ,               IMSA_CallProcSpmMsgStartDtmf},
    {ID_SPM_IMSA_CALL_STOP_DTMF_REQ,                IMSA_CallProcSpmMsgStopDtmf},
    {ID_SPM_IMSA_CALL_GET_CALL_INFO_REQ,            IMSA_CallProcSpmMsgGetInfo},
    {ID_SPM_IMSA_CALL_GET_CLPR_REQ,                 IMSA_CallProcSpmMsgGetClprInfo},
    {ID_SPM_IMSA_CALL_MODIFY_REQ,                   IMSA_CallProcSpmMsgModifyReq},
    {ID_SPM_IMSA_CALL_ANSWER_REMOTE_MODIFY_REQ,     IMSA_CallProcSpmMsgAnswerRemoteModifyReq},
    {ID_SPM_IMSA_PROCESS_USSD_REQ,                  IMSA_SsProcSpmMsgProcessUssdReq},
    {ID_SPM_IMSA_RELEASE_REQ,                       IMSA_SsProcSpmMsgReleaseReq},
    {ID_SPM_IMSA_CALL_INVITE_NEW_PTPT_REQ,          IMSA_CallProcSpmMsgInviteNewPtpt},
    {ID_SPM_IMSA_CALL_ECONF_DIAL_REQ,               IMSA_CallProcSpmMsgEconfDial},
    {ID_SPM_IMSA_CALL_ECONF_ADD_USERS_REQ,          IMSA_CallProcSpmMsgEconfAddUser},
    {ID_SPM_IMSA_CALL_GET_ECONF_CALLED_INFO_REQ,    IMSA_CallProcSpmMsgGetEconfInfo}
};
VOS_UINT32        g_ulImsaSpmCallMsgActTblLen = sizeof(g_stImsaSpmCallMsgActTbl)
                                                / sizeof(IMSA_SPM_CALL_MSG_ACT_STRU);


/*****************************************************************************
  3 Function
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/

VOS_UINT32 IMSA_CallSendSpmClprCnf
(
    const IMSA_SPM_CALL_GET_CLPR_CNF_STRU  *pstMsg
)
{
    IMSA_SPM_CALL_GET_CLPR_CNF_STRU *pstSpmCallMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmClprCnf is entered!");

    /* 分配空间并检验分配是否成功 */
    pstSpmCallMsg = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_GET_CLPR_CNF_STRU));
    if (VOS_NULL_PTR == pstSpmCallMsg)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmClprCnf: alloc memory fail");
        return VOS_FALSE;
    }

    /* 拷贝消息内容 */
    IMSA_MEM_CPY(IMSA_GET_MSG_ENTITY(pstSpmCallMsg), IMSA_GET_MSG_ENTITY(pstMsg), IMSA_GET_MSG_LENGTH(pstSpmCallMsg));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstSpmCallMsg, ID_IMSA_SPM_CALL_GET_CLPR_CNF);

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstSpmCallMsg);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcSpmMsgGetClprInfo
(
    VOS_VOID     *pMsg
)
{
    IMSA_SPM_CALL_GET_CLPR_CNF_STRU     stGetClprCnf    = {0};
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT32                          ulResult        = VOS_FALSE;
    SPM_IMSA_CALL_GET_CLPR_REQ_STRU     *pstAppMsg      = VOS_NULL_PTR;

    pstAppMsg = (SPM_IMSA_CALL_GET_CLPR_REQ_STRU*)pMsg;

    IMSA_MEM_SET(&stGetClprCnf, 0x00, sizeof(IMSA_SPM_CALL_GET_CLPR_CNF_STRU));

    /* 返回MN_CALL_EVT_CLPR_GET_CNF消息 */
    stGetClprCnf.usClientId     = pstAppMsg->usClientId;
    stGetClprCnf.ulRet          = TAF_ERR_ERROR;

    /* 检查输入的CALLID是否存在,并且为被叫 */
    pstCallEntity = IMSA_CallEntityGetUsedBySpmCallId(pstAppMsg->callId);
    if (VOS_NULL_PTR != pstCallEntity)
    {
        IMSA_CallReadCallEntityRedirectInfo(pstCallEntity, &stGetClprCnf);
    }

    ulResult = IMSA_CallSendSpmClprCnf(&stGetClprCnf);

    return ulResult;
}


VOS_UINT32 IMSA_CallSendSpmClccInfoCnf
(
    const IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU *pstMsg
)
{
    IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU *pstSpmCallMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmClccInfoCnf is entered!");

    /* 分配空间并检验分配是否成功 */
    pstSpmCallMsg = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU));
    if (VOS_NULL_PTR == pstSpmCallMsg)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmClccInfoCnf: alloc memory fail");
        return VOS_FALSE;
    }

    /* 拷贝消息内容 */
    IMSA_MEM_CPY(IMSA_GET_MSG_ENTITY(pstSpmCallMsg), IMSA_GET_MSG_ENTITY(pstMsg), IMSA_GET_MSG_LENGTH(pstSpmCallMsg));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstSpmCallMsg, ID_IMSA_SPM_CALL_GET_CALL_INFO_CNF);

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstSpmCallMsg);

    return VOS_TRUE;
}

VOS_UINT32 IMSA_CallSendSpmGetEconfInfoCnf
(
    const IMSA_SPM_CALL_GET_ECONF_CALLED_INFO_CNF_STRU *pstMsg
)
{
    IMSA_SPM_CALL_GET_ECONF_CALLED_INFO_CNF_STRU *pstSpmEconfInfoMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmGetEconfInfoCnf is entered!");

    /* 分配空间并检验分配是否成功 */
    pstSpmEconfInfoMsg = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_GET_ECONF_CALLED_INFO_CNF_STRU));
    if (VOS_NULL_PTR == pstSpmEconfInfoMsg)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmGetEconfInfoCnf: alloc memory fail");
        return VOS_FALSE;
    }

    /* 拷贝消息内容 */
    IMSA_MEM_CPY(IMSA_GET_MSG_ENTITY(pstSpmEconfInfoMsg), IMSA_GET_MSG_ENTITY(pstMsg), IMSA_GET_MSG_LENGTH(pstSpmEconfInfoMsg));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstSpmEconfInfoMsg, ID_IMSA_SPM_CALL_GET_ECONF_CALLED_INFO_CNF);

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstSpmEconfInfoMsg);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcSpmMsgGetInfo
(
    VOS_VOID *pMsg
)
{
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();
    IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU stQryCnf = {0};
    VOS_UINT32 ulCount = 0;
    VOS_UINT32 i = 0;
    VOS_UINT32 ulResult = VOS_FALSE;
    SPM_IMSA_CALL_GET_CALL_INFO_REQ_STRU *pstAppMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgGetInfo is entered!");

    pstAppMsg = (SPM_IMSA_CALL_GET_CALL_INFO_REQ_STRU *)pMsg;

    for (i = 0; i < IMSA_CALL_MAX_NUM; i++)
    {
        if (VOS_TRUE == pstCallCtx->astCallEntity[i].bitOpIsUsed)
        {
            if ((IMSA_CALL_STATUS_IDLE == pstCallCtx->astCallEntity[i].enStatus) &&
                (MN_CALL_DIR_MT == pstCallCtx->astCallEntity[i].enDir))
            {
                continue;
            }
            IMSA_CallEntity2SpmCallInfoParam(&pstCallCtx->astCallEntity[i], &stQryCnf.astCallInfo[ulCount]);
            ulCount++;
        }
    }

    stQryCnf.ucNumOfCalls = (VOS_UINT8)ulCount;
    stQryCnf.usClientId = pstAppMsg->usClientId;
    stQryCnf.enReqType = pstAppMsg->enReqType;


    ulResult = IMSA_CallSendSpmClccInfoCnf(&stQryCnf);

    return ulResult;
}

VOS_UINT32 IMSA_SpmGetCallInfo(IMSA_SPM_CALL_GET_CALL_INFO_IND_STRU *pstCallInfo)
{
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();
    VOS_UINT32 ulCount = 0;
    VOS_UINT32 i = 0;

    IMSA_INFO_LOG("IMSA_SpmGetCallInfo is entered!");

    if (VOS_NULL_PTR == pstCallInfo)
    {
        IMSA_ERR_LOG("IMSA_SpmGetCallInfo: pstCallInfo is null");
        return IMSA_FAIL;
    }

    for (i = 0; i < IMSA_CALL_MAX_NUM; i++)
    {
        if (VOS_TRUE == pstCallCtx->astCallEntity[i].bitOpIsUsed)
        {
            IMSA_CallEntity2SpmCallInfoParam(&pstCallCtx->astCallEntity[i], &(pstCallInfo->astCallInfo[ulCount]));
            ulCount++;
        }
    }

    pstCallInfo->ucNumOfCalls = (VOS_UINT8)ulCount;
    return IMSA_SUCC;
}


VOS_UINT32 IMSA_CallSendSpmStartDtmfCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_START_DTMF_CNF_STRU *pstStartDtmfCnf = VOS_NULL_PTR;

    (VOS_VOID)ucCallId;

    IMSA_INFO_LOG("IMSA_CallSendSpmStartDtmfCnfMsg is entered!");

    /* 分配空间并检验分配是否成功 */
    pstStartDtmfCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_START_DTMF_CNF_STRU));
    if (VOS_NULL_PTR == pstStartDtmfCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmStartDtmfCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* 清空 */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstStartDtmfCnf), 0, IMSA_GET_MSG_LENGTH(pstStartDtmfCnf));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstStartDtmfCnf, ID_IMSA_SPM_CALL_START_DTMF_CNF);

    pstStartDtmfCnf->usClientId = usClientId;
    pstStartDtmfCnf->ucOpId     = ucOpId;
    pstStartDtmfCnf->enCause    = ulResult;

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstStartDtmfCnf);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallSendSpmStopDtmfCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_STOP_DTMF_CNF_STRU   *pstStopDtmfCnf = VOS_NULL_PTR;

    (VOS_VOID)ucCallId;

    IMSA_INFO_LOG("IMSA_CallSendSpmStopDtmfCnfMsg is entered!");

    /* 分配空间并检验分配是否成功 */
    pstStopDtmfCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_STOP_DTMF_CNF_STRU));
    if (VOS_NULL_PTR == pstStopDtmfCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmStopDtmfCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* 清空 */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstStopDtmfCnf), 0, IMSA_GET_MSG_LENGTH(pstStopDtmfCnf));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstStopDtmfCnf, ID_IMSA_SPM_CALL_STOP_DTMF_CNF);

    pstStopDtmfCnf->usClientId = usClientId;
    pstStopDtmfCnf->ucOpId     = ucOpId;
    pstStopDtmfCnf->enCause    = ulResult;

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstStopDtmfCnf);

    return VOS_TRUE;
}



VOS_UINT32 IMSA_CallProcSpmMsgStartDtmf
(
    VOS_VOID* pMsg
)
{
    TAF_CALL_DTMF_PARAM_STRU            stStartDtmfParam    = {0};
    VOS_UINT32                          ulResult            = VOS_FALSE;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx          = IMSA_CallCtxGet();
    SPM_IMSA_CALL_START_DTMF_REQ_STRU           *pstAppMsg  = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgStartDtmf is entered!");

    pstAppMsg = (SPM_IMSA_CALL_START_DTMF_REQ_STRU*)pMsg;

    IMSA_MEM_CPY(   &stStartDtmfParam,
                    &pstAppMsg->stDtmf,
                    sizeof(TAF_CALL_DTMF_PARAM_STRU));

    /* DTMF信息合法性判断 */
    if ( (VOS_OK != IMSA_CallCheckDtmfKey(&stStartDtmfParam.cKey))
      || (stStartDtmfParam.usOnLength < IMSA_DTMF_MIN_ONLENGTH) )
    {
        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_INVALID_PARAMETER);
        return VOS_TRUE;
    }

    /* 先判断当前是否允许发起DTMF，再获取所有可以发送DTMF的呼叫ID */
    ulResult    = IMSA_CallCheckUserDtmfCallId(stStartDtmfParam.CallId);
    if (TAF_CS_CAUSE_SUCCESS != ulResult)
    {
        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    ulResult);
        return VOS_TRUE;
    }

    /* 判断DTMF状态是否正常 */
    if (IMSA_CallGetDtmfState() >= IMSA_CALL_DTMF_STATE_BUTT)
    {
        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_STATE_ERROR);
        return VOS_TRUE;
    }

    /* DTMF处于非空闲状态，则缓存此DTMF */
    if (IMSA_CallGetDtmfState() != IMSA_CALL_DTMF_IDLE)
    {
        ulResult = IMSA_CallSaveDtmfInfo(pstAppMsg->stDtmf.CallId,
                                         pstAppMsg->stDtmf.cKey,
                                         pstAppMsg->stDtmf.usOnLength,
                                         pstAppMsg->usClientId,
                                         pstAppMsg->ucOpId);

        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(pstAppMsg->usClientId,
                                                  pstAppMsg->ucOpId,
                                                  pstAppMsg->stDtmf.CallId,
                                                  ulResult);
        return VOS_TRUE;
    }

    /* DTMF当前处于空闲态，则发送到IMS协议栈 */
    if (VOS_TRUE != IMSA_CallSendImsMsgStartDtmf(   pstAppMsg->stDtmf.CallId,
                                                    stStartDtmfParam.cKey,
                                                    (VOS_UINT32)stStartDtmfParam.usOnLength))
    {
        IMSA_ERR_LOG("IMSA_CallProcSpmMsgStartDtmf: error occurs, should return failure to SPM");

        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_IMSA_ERROR);

        return VOS_TRUE;
    }

    /* 发送成功，更新DTMF状态 */
    IMSA_StartTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);

    IMSA_CallFillCurrentDtmfInfo(   pstAppMsg->stDtmf.CallId,
                                    stStartDtmfParam.cKey,
                                    stStartDtmfParam.usOnLength,
                                    pstAppMsg->usClientId,
                                    pstAppMsg->ucOpId);

    IMSA_CallSetDtmfState(IMSA_CALL_DTMF_WAIT_START_CNF);

    (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                pstAppMsg->ucOpId,
                                                pstAppMsg->stDtmf.CallId,
                                                TAF_CS_CAUSE_SUCCESS);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcSpmMsgStopDtmf
(
    VOS_VOID* pMsg
)
{
    VOS_UINT32                          ulResult            = VOS_FALSE;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx          = IMSA_CallCtxGet();
    TAF_CALL_DTMF_PARAM_STRU            stStopDtmfParam     = {0};
    IMSA_CALL_DTMF_STATE_ENUM_UINT8     enDtmfState         = IMSA_CALL_DTMF_STATE_BUTT;
    SPM_IMSA_CALL_STOP_DTMF_REQ_STRU         *pstAppMsg     = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgStopDtmf is entered!");

    pstAppMsg = (SPM_IMSA_CALL_STOP_DTMF_REQ_STRU*)pMsg;

    IMSA_MEM_CPY(   &stStopDtmfParam,
                    &pstAppMsg->stDtmf,
                    sizeof(TAF_CALL_DTMF_PARAM_STRU));

    /* STOP_DTMF请求时，OnLength值不为0 */
    if (0 != stStopDtmfParam.usOnLength)
    {
        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_INVALID_PARAMETER);
        return VOS_TRUE;
    }

    /* 判断DTMF状态是否正常 */
    enDtmfState = IMSA_CallGetDtmfState();
    if ( (IMSA_CALL_DTMF_STATE_BUTT <= enDtmfState)
      || (IMSA_CALL_DTMF_IDLE == enDtmfState) )
    {
        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_STATE_ERROR);
        return VOS_TRUE;
    }

    /* 先判断当前是否允许发起DTMF，以及用户输入的呼叫ID是否可以发送DTMF */
    ulResult    = IMSA_CallCheckUserDtmfCallId(stStopDtmfParam.CallId);
    if (TAF_CS_CAUSE_SUCCESS != ulResult)
    {
        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    ulResult);
        return VOS_TRUE;
    }

    /* 如果当前没有缓存且处于等待定时器超时状态，则停定时器，发送STOP_DTMF，否则缓存 */
    if ((IMSA_CALL_DTMF_WAIT_TIME_OUT == enDtmfState)
        && (0 == IMSA_CallGetDtmfCnt()))
    {
        /* 停DTMF duration定时器 */
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfDurationTimer);

        if (VOS_TRUE != IMSA_CallSendImsMsgStopDtmf(pstAppMsg->stDtmf.CallId,
                                                    stStopDtmfParam.cKey))
        {
            IMSA_ERR_LOG("IMSA_CallProcSpmMsgStopDtmf: error occurs, should return failure to SPM");

            (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                        pstAppMsg->ucOpId,
                                                        pstAppMsg->stDtmf.CallId,
                                                        TAF_CS_CAUSE_IMSA_ERROR);
            return VOS_TRUE;
        }

        /* 发送成功，更新DTMF状态 */
        IMSA_StartTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);

        IMSA_CallFillCurrentDtmfInfo(   pstAppMsg->stDtmf.CallId,
                                        stStopDtmfParam.cKey,
                                        stStopDtmfParam.usOnLength,
                                        pstAppMsg->usClientId,
                                        pstAppMsg->ucOpId);

        IMSA_CallSetDtmfState(IMSA_CALL_DTMF_WAIT_STOP_CNF);

        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_SUCCESS);
        return VOS_TRUE;
    }

    /* 当前不能发送STOP_DTMF请求，则缓存 */
    ulResult = IMSA_CallSaveDtmfInfo(pstAppMsg->stDtmf.CallId,
                                     pstAppMsg->stDtmf.cKey,
                                     pstAppMsg->stDtmf.usOnLength,
                                     pstAppMsg->usClientId,
                                     pstAppMsg->ucOpId);

    (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(pstAppMsg->usClientId,
                                             pstAppMsg->ucOpId,
                                             pstAppMsg->stDtmf.CallId,
                                             ulResult);

    return VOS_TRUE;
}
VOS_UINT32 IMSA_CallSendSpmSupsCnfMsg(VOS_UINT16 usClientId,
                                      VOS_UINT32 ulOpId,
                                      VOS_UINT32 ulCallId,
                                      TAF_CS_CAUSE_ENUM_UINT32 ulResult)
{
    IMSA_INFO_LOG("IMSA_CallSendSpmSupsCnfMsg is entered!");

    return IMSA_CallSendSpmEvtAndCallInfo(usClientId,
                                          ulOpId,
                                          ulCallId,
                                          MN_CALL_EVT_SUPS_CMD_CNF,
                                          ulResult);
}


VOS_UINT32 IMSA_CallProcSpmMsgSups(VOS_VOID* pMsg)
{
    VOS_UINT32 ulResult = VOS_TRUE;
    VOS_UINT32 ulCallId = 0;
    SPM_IMSA_CALL_SUPS_CMD_REQ_STRU *pstAppMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgSups is entered!");

    pstAppMsg = (SPM_IMSA_CALL_SUPS_CMD_REQ_STRU *)pMsg;


    ulCallId = pstAppMsg->stCallMgmtCmd.callId;

    IMSA_INFO_LOG1("IMSA_CallProcSpmMsgSups", pstAppMsg->stCallMgmtCmd.enCallSupsCmd);

    switch (pstAppMsg->stCallMgmtCmd.enCallSupsCmd)
    {
        /* CHLD=0, Releases all held calls or sets User Determined User Busy (UDUB) for a waiting call */
        case MN_CALL_SUPS_CMD_REL_HELD_OR_UDUB:
            ulResult = IMSA_CallProcSpmMsgSupsRelHeldOrUdub(ulCallId, pstAppMsg);
            break;
        /* CHLD=1, Releases all active calls (if any exist) and accepts the other (held or waiting) call */
        case MN_CALL_SUPS_CMD_REL_ACT_ACPT_OTH:
            ulResult = IMSA_CallProcSpmMsgSupsRelActAcptOth(ulCallId, pstAppMsg);
            break;
        /* CHLD=1x, Releases a specific active call X*/
        case MN_CALL_SUPS_CMD_REL_CALL_X:
            ulResult = IMSA_CallProcSpmMsgSupsRelCallX(ulCallId, pstAppMsg);
            break;
        /* ATH, Release all calls */
        case MN_CALL_SUPS_CMD_REL_ALL_CALL:
            ulResult = IMSA_CallProcSpmMsgSupsRelAllCall(ulCallId, pstAppMsg);
            break;
        /* CHLD=2, Places all active calls (if any exist) on hold and accepts the other (held or waiting) call */
        case MN_CALL_SUPS_CMD_HOLD_ACT_ACPT_OTH:
            ulResult = IMSA_CallProcSpmMsgSupsHoldActAcptOth(ulCallId, pstAppMsg);
            break;
        /* CHLD=2x, Places all active calls on hold except call X with which communication shall be supported */
        case MN_CALL_SUPS_CMD_HOLD_ALL_EXCPT_X:
            ulResult = IMSA_CallProcSpmMsgSupsHoldAllExpectX(ulCallId, pstAppMsg);
            break;
        /* CHLD=3, Adds a held call to the conversation */
        case MN_CALL_SUPS_CMD_BUILD_MPTY:
            ulResult = IMSA_CallProcSpmMsgSupsBuidMpty(ulCallId, pstAppMsg);
            break;
        /* reject unsupported SPM command */
        case MN_CALL_SUPS_CMD_ECONF_REL_USER:
            ulResult = IMSA_CallProcSpmMsgSupsEconfRelUser(ulCallId, pstAppMsg);
            break;
        case MN_CALL_SUPS_CMD_ECONF_MERGE_CALL:
            ulResult = IMSA_CallProcSpmMsgSupsEconfRelUser(ulCallId, pstAppMsg);
            break;
        default:
            ulResult = VOS_FALSE;
            IMSA_ERR_LOG("IMSA_CallProcSpmMsgSups: not support sups message");

            (VOS_VOID)IMSA_CallSendSpmSupsCnfMsg(pstAppMsg->usClientId,
                                                 pstAppMsg->ucOpId,
                                                 pstAppMsg->stCallMgmtCmd.callId,
                                                 TAF_CS_CAUSE_IMSA_NOT_SUPPORTED_CMD);
    }

    return ulResult;
}


VOS_UINT32 IMSA_CallSendSpmOrigCnfMsg(VOS_UINT16 usClientId,
                                      VOS_UINT32 ulOpId,
                                      VOS_UINT32 ulCallId,
                                      TAF_CS_CAUSE_ENUM_UINT32 ulResult)
{
    IMSA_INFO_LOG("IMSA_CallSendSpmOrigCnfMsg is entered!");

    return IMSA_CallSendSpmEvtAndCallInfo(usClientId,
                                          ulOpId,
                                          ulCallId,
                                          MN_CALL_EVT_CALL_ORIG_CNF,
                                          ulResult);
}

VOS_UINT32 IMSA_CallSendSpmEconfDialCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT32                          ulOpId,
    VOS_UINT32                          ulCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_ECONF_DIAL_CNF_STRU *pstEconfDialCnf = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmEconfDialCnfMsg is entered!");

    /* 分配空间并检验分配是否成功 */
    pstEconfDialCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_ECONF_DIAL_CNF_STRU));
    if (VOS_NULL_PTR == pstEconfDialCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmEconfDialCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* 清空 */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstEconfDialCnf), 0, IMSA_GET_MSG_LENGTH(pstEconfDialCnf));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstEconfDialCnf, ID_IMSA_SPM_CALL_ECONF_DIAL_CNF);

    pstEconfDialCnf->usClientId = usClientId;
    pstEconfDialCnf->ucOpId     = (MN_OPERATION_ID_T)ulOpId;
    pstEconfDialCnf->enCause    = ulResult;
    pstEconfDialCnf->ucCallId   = (VOS_UINT8)ulCallId;

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstEconfDialCnf);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallSendSpmEconfAddUserCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT32                          ulOpId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_ECONF_ADD_USERS_CNF_STRU *pstEconfAddUserCnf = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmEconfAddUserCnfMsg is entered!");

    /* 分配空间并检验分配是否成功 */
    pstEconfAddUserCnf= (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_ECONF_ADD_USERS_CNF_STRU));
    if (VOS_NULL_PTR == pstEconfAddUserCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmEconfAddUserCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* 清空 */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstEconfAddUserCnf), 0, IMSA_GET_MSG_LENGTH(pstEconfAddUserCnf));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstEconfAddUserCnf, ID_IMSA_SPM_CALL_ECONF_ADD_USERS_CNF);

    pstEconfAddUserCnf->usClientId = usClientId;
    pstEconfAddUserCnf->ucOpId     = (MN_OPERATION_ID_T)ulOpId;
    pstEconfAddUserCnf->enCause    = ulResult;

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstEconfAddUserCnf);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcSpmMsgOrig(VOS_VOID* pMsg)
{
    VOS_UINT32                          ulCheckRet      = 0;
    VOS_UINT32                          ulResult        = 0;
    MN_CALL_ORIG_PARAM_STRU            *pstOrigParam    = VOS_NULL_PTR;
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();
    IMSA_EMC_CALL_TYPE_ENUM_UINT32      enEmcType       = IMSA_EMC_CALL_TYPE_BUTT;
    SPM_IMSA_CALL_ORIG_REQ_STRU         *pstAppMsg      = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgOrig is entered!");

    pstAppMsg = (SPM_IMSA_CALL_ORIG_REQ_STRU *)pMsg;

    pstOrigParam = &pstAppMsg->stOrig;

    /* 目前只支持语音呼叫和紧急呼叫和视频呼叫三种类型 */
    if (VOS_FALSE == IMSA_CallTypeAvailabilityCheck(pstOrigParam->enCallType))
    {
        (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                             pstAppMsg->ucOpId,
                                             IMSA_CALL_INVALID_ID,
                                             TAF_CS_CAUSE_IMSA_NOT_SUPPORTED_CALL_TYPE);

        #if (FEATURE_ON == FEATURE_PTM)
        IMSA_CallErrRecord(IMSA_ERR_LOG_CALL_FAIL_REASON_NOT_SUPPORTED_CALL_TYPE, VOS_NULL_PTR);
        #endif
        return VOS_FALSE;
    }

    if (VOS_FALSE == IMSA_CallCheckIsAllowNewOrig())
    {
        (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                             pstAppMsg->ucOpId,
                                             IMSA_CALL_INVALID_ID,
                                             TAF_CS_CAUSE_IMSA_SERVICE_NOT_AVAILABLE);
        return VOS_FALSE;
    }

    /* 进行呼叫可用性检查 */
    ulCheckRet = IMSA_CallAvailabilityCheck(pstOrigParam->enCallType, &enEmcType);

    IMSA_INFO_LOG1("IMSA_CallProcSpmMsgOrig: IMSA_CallAvailabilityCheck result", ulCheckRet);

    /* 如果检查结果为可以继续发起呼叫，或者需要先发起紧急注册，则先分配呼叫实体，给SPM回复ORIG_CNF，
    同时给SPM发送ORIG事件，因为SPM需要通过该事件去更新callid，防止在IMSA还未收到IMS的DIAL消息时，收到
    用户的挂断命令，如果IMSA在上报ORIG事件前，直接报RELEASE事件，SPM由于没有callid，无法处理 */
    if ((IMSA_CALL_AVAILABILITY_CHECK_RESULT_CONTINUE == ulCheckRet) ||
        (IMSA_CALL_AVAILABILITY_CHECK_RESULT_CACHED == ulCheckRet))
    {

        pstCallEntity = IMSA_CallEntityAlloc();
        if (VOS_NULL_PTR != pstCallEntity)
        {
            /* 保存是否已经在CS域尝试过的标识 */
            pstCallEntity->bitOpRetryCsCall = pstOrigParam->ucCsCallRetryFlg;

            /* 保存呼叫模式 */
            pstCallEntity->enMode = pstOrigParam->enCallMode;

            /* 保存紧急呼类型 */
            pstCallEntity->enEmcType = enEmcType;

            /* 保存CLIENT ID和OPID */
            pstCallEntity->usClientId = pstAppMsg->usClientId;
            pstCallEntity->ulOpId = pstAppMsg->ucOpId;

            /* 保存CALL类型 */
            pstCallEntity->enType = (IMSA_CALL_TYPE_ENUM_UINT8)pstAppMsg->stOrig.enCallType;

            /* 更新被叫号码 */
            IMSA_CallEntityUpdateCalledNumberByBcd(&(pstAppMsg->stOrig.stDialNumber), &pstCallEntity->stCalledNumber);


            /* 给SPM回复ORIG_CNF */
            (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                                 pstAppMsg->ucOpId,
                                                 pstCallEntity->ucSpmcallId,
                                                 TAF_CS_CAUSE_SUCCESS);

            /* 给SPM回复ORIG事件 */
            (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucSpmcallId,
                                                pstCallEntity->usClientId,
                                                pstCallEntity->ulOpId,
                                                MN_CALL_EVT_ORIG);
        }
        else
        {
            /* 分配资源失败，拒绝该呼叫请求 */
            (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                                 pstAppMsg->ucOpId,
                                                 IMSA_CALL_INVALID_ID,
                                                 TAF_CS_CAUSE_IMSA_ALLOC_ENTITY_FAIL);

            #if (FEATURE_ON == FEATURE_PTM)
            IMSA_CallErrRecord(IMSA_ERR_LOG_CALL_FAIL_REASON_SERVICE_NOT_AVAILABLE, VOS_NULL_PTR);
            #endif

            return VOS_TRUE;
        }
    }
    else
    {
        /* 视频呼叫被罢，将视频呼叫切换为语音呼叫 */
        if (MN_CALL_TYPE_VIDEO == pstAppMsg->stOrig.enCallType)
        {
            (VOS_VOID)IMSA_CallSendSpmCallTypeChangeInfoIndMsg(pstAppMsg->usClientId,
                                                               pstAppMsg->ucOpId,
                                                               IMSA_CALL_INVALID_ID,
                                                               MN_CALL_TYPE_VIDEO,
                                                               MN_CALL_TYPE_VOICE,
                                                               IMSA_IMS_EMERGENCY_SUB_TYPE_NONE);
        }

        (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                             pstAppMsg->ucOpId,
                                             IMSA_CALL_INVALID_ID,
                                             TAF_CS_CAUSE_IMSA_SERVICE_NOT_AVAILABLE);

        #if (FEATURE_ON == FEATURE_PTM)
        if (VOS_FALSE == IMSA_CallCheckIsAllowNewOrig())
        {
            IMSA_CallErrRecord(IMSA_ERR_LOG_CALL_FAIL_REASON_NOT_ALLOW_NEW_CALL, VOS_NULL_PTR);
        }
        else
        {
            IMSA_CallErrRecord(IMSA_ERR_LOG_CALL_FAIL_REASON_SERVICE_NOT_AVAILABLE, VOS_NULL_PTR);
        }
        #endif
        return VOS_TRUE;
    }

    if(ulCheckRet == IMSA_CALL_AVAILABILITY_CHECK_RESULT_CONTINUE)
    {
        /* Send IMS command */
        ulResult = IMSA_CallSendImsMsgDial(IMSA_CALL_INVALID_ID, pstAppMsg);
        if (VOS_TRUE == ulResult)
        {
            /* 保存触发此呼叫对应的SPM命令，如果呼叫被拒绝且需要紧急注册后再重试时使用 */
            IMSA_MEM_CPY(   &pstCallEntity->stSpmMsg,
                            &pstCallCtx->stSpmMsg,
                            sizeof(IMSA_CALL_SPM_MSG_STRU));

            /* 启动保护定时器 */
            IMSA_StartTimer(&pstCallCtx->stProctectTimer);

            /* 启动重播最大时长定时器 */
            pstCallCtx->stRedialMaxTimer.ulTimerLen = IMSA_GetImsRedialCfgAddress()->ulCallRedialMaxTime;
            pstCallCtx->stRedialMaxTimer.usPara = pstCallEntity->ucCallEntityIndex;
            IMSA_StartTimer(&pstCallCtx->stRedialMaxTimer);
            IMSA_INFO_LOG1("Start redial max timer tick:", PS_GET_TICK());
            IMSA_StartTcallTimer(pstCallEntity);
        }
        else
        {
            /* 通知SPM release事件 */
            IMSA_CallReleaseCallCommonProc(pstCallEntity, TAF_CS_CAUSE_IMSA_ERROR);
            #if (FEATURE_ON == FEATURE_PTM)
            IMSA_CallErrRecord(IMSA_ERR_LOG_CALL_FAIL_REASON_SERVICE_NOT_AVAILABLE, VOS_NULL_PTR);
            #endif
        }
    }
    else/* 首先发起服务，需要缓存呼叫命令，待服务成功后再处理缓存消息 */
    {
        IMSA_CallSpmOrigReqMsgSave(pstAppMsg);
        IMSA_StartTcallTimer(pstCallEntity);
    }

    return VOS_TRUE;
}

VOS_UINT32 IMSA_CallProcSpmMsgEconfDial(VOS_VOID *pMsg)
{
    VOS_UINT32                          ulCheckRet      = 0;
    VOS_UINT32                          ulResult        = 0;
    TAF_CALL_ECONF_DIAL_REQ_STRU        *pstDialParam    = VOS_NULL_PTR;
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();
    IMSA_EMC_CALL_TYPE_ENUM_UINT32      enEmcType       = IMSA_EMC_CALL_TYPE_BUTT;
    VOS_UINT8                           i = 0;
    SPM_IMSA_CALL_ECONF_DIAL_REQ_STRU *pstAppMsg        = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgEconfDial is entered!");

    pstAppMsg = (SPM_IMSA_CALL_ECONF_DIAL_REQ_STRU*)pMsg;

    pstDialParam = &pstAppMsg->stDialReq;

    /* 目前只支持语音类型的增强多方通话 */
    if (MN_CALL_TYPE_VOICE != pstDialParam->enCallType)
    {
        (VOS_VOID)IMSA_CallSendSpmEconfDialCnfMsg(pstAppMsg->usClientId,
                                                  pstAppMsg->ucOpId,
                                                  IMSA_CALL_INVALID_ID,
                                                  TAF_CS_CAUSE_IMSA_NOT_SUPPORTED_CALL_TYPE);

        return VOS_FALSE;
    }

    /* 进行呼叫可用性检查 */
    ulCheckRet = IMSA_CallAvailabilityCheck(pstDialParam->enCallType, &enEmcType);

    IMSA_INFO_LOG1("IMSA_CallProcSpmMsgEconfDial: IMSA_CallAvailabilityCheck result", ulCheckRet);

    if ((IMSA_CALL_AVAILABILITY_CHECK_RESULT_CONTINUE == ulCheckRet) &&
        (VOS_TRUE == IMSA_CallCheckIsAllowNewOrig()))
    {

        pstCallEntity = IMSA_CallEntityAlloc();
        if (VOS_NULL_PTR != pstCallEntity)
        {
            /* 保存呼叫模式 */
            pstCallEntity->enMode = pstDialParam->enCallMode;

            /* 保存紧急呼类型 */
            pstCallEntity->enEmcType = enEmcType;

            /* 设置增强多方通话的标志位 */
            pstCallEntity->ucIsEconfFlag = VOS_TRUE;

            /* 更新多方通话的模式 */
            pstCallEntity->enMpty = MN_CALL_IN_MPTY;

            /* 保存CLIENT ID和OPID */
            pstCallEntity->usClientId = pstAppMsg->usClientId;
            pstCallEntity->ulOpId = pstAppMsg->ucOpId;

            pstCallEntity->enType = (IMSA_CALL_TYPE_ENUM_UINT8)pstAppMsg->stDialReq.enCallType;

            pstCallEntity->ucCurUserNum = (VOS_UINT8)pstAppMsg->stDialReq.stEconfCalllist.ulCallNum;

            /* 更新与会者成员列表 */
            for (i = 0; i < pstCallEntity->ucCurUserNum; i++)
            {
                 IMSA_CallEntityUpdateCalledNumberByBcd(&(pstAppMsg->stDialReq.stEconfCalllist.astDialNumber[i]), &pstCallEntity->stEconfCalllist[i].stCalledNumber);

                pstCallEntity->stEconfCalllist[i].enCallState = IMSA_IMS_ECONF_CALLER_STATE_PREDIALING;
            }

            /* 给SPM回复ORIG_CNF */
            (VOS_VOID)IMSA_CallSendSpmEconfDialCnfMsg(pstAppMsg->usClientId,
                                                      pstAppMsg->ucOpId,
                                                      pstCallEntity->ucSpmcallId,
                                                      TAF_CS_CAUSE_SUCCESS);

            /* 给SPM回复ORIG事件 */
            (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucSpmcallId,
                                                pstCallEntity->usClientId,
                                                pstCallEntity->ulOpId,
                                                MN_CALL_EVT_ORIG);

            /* 优化增强型第三方通话流程中无Alerting, 所以在此处通知LRRC Start, LRRC收到该消息后,
            当信号质量不好时将提前上报A2/B2事件,触发网侧提前下发Srvcc */
            if (VOS_FALSE == g_ulImsaNotifyRrcVoLteCallStartFlag)
            {
                IMSA_SndRrcVolteStatusNotify(IMSA_LRRC_VOLTE_STATUS_START);
            }
        }
        else
        {
            /* 分配资源失败，拒绝该呼叫请求 */
            (VOS_VOID)IMSA_CallSendSpmEconfDialCnfMsg(pstAppMsg->usClientId,
                                                     pstAppMsg->ucOpId,
                                                     IMSA_CALL_INVALID_ID,
                                                     TAF_CS_CAUSE_IMSA_ALLOC_ENTITY_FAIL);
            return VOS_TRUE;
        }
    }
    else
    {
        (VOS_VOID)IMSA_CallSendSpmEconfDialCnfMsg(pstAppMsg->usClientId,
                                                 pstAppMsg->ucOpId,
                                                 IMSA_CALL_INVALID_ID,
                                                 TAF_CS_CAUSE_IMSA_SERVICE_NOT_AVAILABLE);
        return VOS_TRUE;
    }

    /* Send IMS command */
    ulResult = IMSA_CallSendImsMsgCreatEconf(IMSA_CALL_INVALID_ID, pstAppMsg);
    if (VOS_TRUE == ulResult)
    {
        /* 保存触发此呼叫对应的SPM命令，如果呼叫被拒绝且需要紧急注册后再重试时使用 */
        IMSA_MEM_CPY(   &pstCallEntity->stSpmMsg,
                        &pstCallCtx->stSpmMsg,
                        sizeof(IMSA_CALL_SPM_MSG_STRU));

        /* 启动保护定时器 */
        IMSA_StartTimer(&pstCallCtx->stProctectTimer);

    }
    else
    {
        /* 通知SPM release事件 */
        IMSA_CallReleaseCallCommonProc(pstCallEntity, TAF_CS_CAUSE_IMSA_ERROR);
    }

    return VOS_TRUE;
}
VOS_UINT32 IMSA_CallProcSpmMsgEconfAddUser(VOS_VOID *pMsg)
{
    VOS_UINT32                          ulResult        = VOS_TRUE;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();
    SPM_IMSA_CALL_ECONF_ADD_USERS_REQ_STRU *pstAppMsg   = VOS_NULL_PTR;
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT8                           ucCallNum       = 0;
    VOS_UINT8                           i = 0;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgEconfAddUser is entered!");

    pstAppMsg = (SPM_IMSA_CALL_ECONF_ADD_USERS_REQ_STRU*)pMsg;
    pstCallEntity = IMSA_CallEntityGetUsedByEconfFlag();

    /* 如果不存在增强多方通话，直接返回失败 */
    if (VOS_NULL_PTR == pstCallEntity)
    {
        /* 需要通知SPM 添加与会者流程失败 */
        IMSA_CallSendSpmEconfAddUserCnfMsg(pstAppMsg->usClientId,
                                           pstAppMsg->ucOpId,
                                           TAF_CS_CAUSE_IMSA_ERROR);
        return VOS_FALSE;
    }

    /* 判断人数是否超过5个，如果超过，直接返回失败 */
    if (IMSA_IMS_ECONF_CALLED_MAX_NUM < pstAppMsg->stEconfCalllist.ulCallNum)
    {
        /* 需要通知SPM 添加与会者流程失败 */
        IMSA_CallSendSpmEconfAddUserCnfMsg(pstAppMsg->usClientId,
                                           pstAppMsg->ucOpId,
                                           TAF_CS_CAUSE_IMSA_ERROR);
        return VOS_FALSE;
    }

    ucCallNum = (VOS_UINT8)(pstCallEntity->ucCurUserNum + pstAppMsg->stEconfCalllist.ulCallNum);
    if (IMSA_IMS_ECONF_CALLED_MAX_NUM < ucCallNum)
    {
        /* 需要通知SPM 添加与会者流程失败 */
        IMSA_CallSendSpmEconfAddUserCnfMsg(pstAppMsg->usClientId,
                                           pstAppMsg->ucOpId,
                                           TAF_CS_CAUSE_IMSA_ERROR);
        return VOS_FALSE;
    }

    /* 在呼叫实体中保存新增的被叫 */
    for (i = 0; i < pstAppMsg->stEconfCalllist.ulCallNum; i++)
    {
        IMSA_CallCopyRemoteAddrForm(&pstAppMsg->stEconfCalllist.astDialNumber[i],
                                     pstCallEntity->stEconfCalllist[pstCallEntity->ucCurUserNum].stCalledNumber.aucNumber);

        pstCallEntity->stEconfCalllist[pstCallEntity->ucCurUserNum].enCallState = IMSA_IMS_ECONF_CALLER_STATE_PREDIALING;

        pstCallEntity->ucCurUserNum++ ;
    }

    ulResult = IMSA_CallSendImsMsgEconfAddUser(IMSA_CALL_INVALID_ID, pstAppMsg);

    if (VOS_TRUE == ulResult)
    {
        /* 启动保护定时器 */
        IMSA_StartTimer(&pstCallCtx->stProctectTimer);
    }
    else
    {
        IMSA_ERR_LOG("IMSA_CallProcSpmMsgEconfAddUser: error occurs, should return failure to SPM");

        /* 需要通知SPM 添加与会者流程失败 */
        IMSA_CallSendSpmEconfAddUserCnfMsg(pstAppMsg->usClientId,
                                           pstAppMsg->ucOpId,
                                           TAF_CS_CAUSE_IMSA_ERROR);
        ulResult = VOS_FALSE;

    }

    return ulResult;

}

VOS_UINT32 IMSA_CallProcSpmMsgGetEconfInfo
(
    VOS_VOID *pMsg
)
{
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    IMSA_SPM_CALL_GET_ECONF_CALLED_INFO_CNF_STRU stQryCnf ={0};
    VOS_UINT32                          ulResult = VOS_FALSE;
    TAF_CS_CAUSE_ENUM_UINT32            enSpmErr        = IMSA_NULL;
    VOS_UINT8                           i= 0;
    SPM_IMSA_CALL_GET_ECONF_CALLED_INFO_REQ_STRU *pstAppMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgGetEconfInfo is entered!");

    pstAppMsg = (SPM_IMSA_CALL_GET_ECONF_CALLED_INFO_REQ_STRU*)pMsg;

    pstCallEntity = IMSA_CallEntityGetUsedByEconfFlag();

    if (VOS_NULL_PTR != pstCallEntity)
    {
        stQryCnf.ucNumOfCalls = pstCallEntity->ucCurUserNum;

        stQryCnf.ucNumOfMaxCalls = pstCallEntity->ucMaxUserNum;

        for (i = 0; i < pstCallEntity->ucCurUserNum; i ++)
        {
            /* 更新与会者状态 */
            stQryCnf.astCallInfo[i].enCallState = IMSA_EconfImsState2CsState(pstCallEntity->stEconfCalllist[i].enCallState);

            /* 更新与会者号码 */
            IMSA_CallCalledNumberImsa2CS(&pstCallEntity->stEconfCalllist[i].stCalledNumber, &stQryCnf.astCallInfo[i].stCallNumber);

            /* 更新display name */
            IMSA_MEM_CPY(stQryCnf.astCallInfo[i].aucDisplaytext,
                         pstCallEntity->stEconfCalllist[i].acAlpha,
                         TAF_IMSA_ALPHA_STRING_SZ);

            /* 更新原因值 */
            if (IMSA_OP_TRUE == pstCallEntity->stEconfCalllist[i].bitOpErrorInfo)
            {
                /* 将IMS错误原因值转化为SPM错误原因值 */
                /* 修改函数入参 */
                enSpmErr = IMSA_CallTransImsErr2SpmErr(&pstCallEntity->stEconfCalllist[i].stErrorCode);

                IMSA_CallReleaseCallCommonProc(pstCallEntity, enSpmErr);
            }
            else
            {
                stQryCnf.astCallInfo[i].enCause = TAF_CS_CAUSE_SUCCESS;
            }

        }

        i = 0;

        /* 如果与会者状态为DISCONNECT状态，则需要在AP侧查询过状态后，将该用户从列表中删除，避免新增用户时，没有
        位置保存新增用户信息 ；手机界面上DISCONNECT状态的用户由AP侧自己维护 */
        while(i < pstCallEntity->ucCurUserNum)
        {
            if (IMSA_IMS_ECONF_CALLER_STATE_DISCONNECT == pstCallEntity->stEconfCalllist[i].enCallState)
            {
                IMSA_MEM_CPY(&pstCallEntity->stEconfCalllist[i],
                            &pstCallEntity->stEconfCalllist[i + 1],
                            (IMSA_IMS_ECONF_CALLED_MAX_NUM - i - 1)*sizeof(IMSA_CALL_ECONF_SUMMARY_STRU));

                pstCallEntity->ucCurUserNum --;
            }
            else
            {
                i++;
            }
        }

    }

    stQryCnf.usClientId = pstAppMsg->usClientId;
    stQryCnf.enReqType = pstAppMsg->enReqType;
    stQryCnf.ucOpId = pstAppMsg->ucOpId;

    ulResult = IMSA_CallSendSpmGetEconfInfoCnf(&stQryCnf);


    return ulResult;
}

/* xiongxianghui00253310 add for conference 20140210 begin */

VOS_UINT32 IMSA_CallSendSpmInviteNewPtptCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT32                          ulOpId,
    VOS_UINT32                          ulCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_INVITE_NEW_PTPT_CNF_STRU *pstInviteNewPtptCnf = VOS_NULL_PTR;

    (VOS_VOID)ulCallId;

    IMSA_INFO_LOG("IMSA_CallSendSpmInviteNewPtptCnfMsg is entered!");

    /* 分配空间并检验分配是否成功 */
    pstInviteNewPtptCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_INVITE_NEW_PTPT_CNF_STRU));
    if (VOS_NULL_PTR == pstInviteNewPtptCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmInviteNewPtptCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* 清空 */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstInviteNewPtptCnf), 0, IMSA_GET_MSG_LENGTH(pstInviteNewPtptCnf));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstInviteNewPtptCnf, ID_IMSA_SPM_CALL_INVITE_NEW_PTPT_CNF);

    pstInviteNewPtptCnf->usClientId = usClientId;
    pstInviteNewPtptCnf->ucOpId     = (MN_OPERATION_ID_T)ulOpId;
    pstInviteNewPtptCnf->enCause    = ulResult;

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstInviteNewPtptCnf);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallSendSpmCallTypeChangeInfoIndMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT32                          ulOpId,
    VOS_UINT32                          ulCallId,
    MN_CALL_TYPE_ENUM_U8                enSrcCallType,
    MN_CALL_TYPE_ENUM_U8                enDestCallType,
    const IMSA_IMS_EMERGENCY_TYPE_ENUM_UINT8    EmcSubType
)
{
    IMSA_SPM_CALL_TYPE_CHANGE_INFO_IND_STRU *pstChangeInfoInd = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmCallTypeChangeInfoIndMsg is entered!");

    /* 分配空间并检验分配是否成功 */
    pstChangeInfoInd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_TYPE_CHANGE_INFO_IND_STRU));
    if (VOS_NULL_PTR == pstChangeInfoInd)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmCallTypeChangeInfoIndMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* 清空 */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstChangeInfoInd), 0, IMSA_GET_MSG_LENGTH(pstChangeInfoInd));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstChangeInfoInd, ID_IMSA_SPM_CALL_TYPE_CHANGE_INFO_IND);

    pstChangeInfoInd->usClientId = usClientId;
    pstChangeInfoInd->ucOpId     = (MN_OPERATION_ID_T)ulOpId;
    pstChangeInfoInd->ucCallId     = (MN_CALL_ID_T)ulCallId;

    pstChangeInfoInd->enSrcCallType = enSrcCallType;
    pstChangeInfoInd->enDestCallType = enDestCallType;

    IMSA_CallSetSpmEmergencyCat(EmcSubType, &(pstChangeInfoInd->stEmergencyCat));

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstChangeInfoInd);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallSendSpmModifyCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT32                          ulOpId,
    VOS_UINT32                          ulCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_MODIFY_CNF_STRU *pstModifyCnf = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmModifyCnfMsg is entered!");

    /* 分配空间并检验分配是否成功 */
    pstModifyCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_MODIFY_CNF_STRU));
    if (VOS_NULL_PTR == pstModifyCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmModifyCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* 清空 */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstModifyCnf), 0, IMSA_GET_MSG_LENGTH(pstModifyCnf));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstModifyCnf, ID_IMSA_SPM_CALL_MODIFY_CNF);

    pstModifyCnf->usClientId = usClientId;
    pstModifyCnf->ucOpId     = (MN_OPERATION_ID_T)ulOpId;
    pstModifyCnf->enCause    = ulResult;
    pstModifyCnf->callId     = ulCallId;

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstModifyCnf);

    return VOS_TRUE;
}

VOS_UINT32 IMSA_CallSendSpmAnswerRemoteModifyCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT32                          ulOpId,
    VOS_UINT32                          ulCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_MODIFY_CNF_STRU *pstModifyCnf = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmAnswerRemoteModifyCnfMsg is entered!");

    /* 分配空间并检验分配是否成功 */
    pstModifyCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_MODIFY_CNF_STRU));
    if (VOS_NULL_PTR == pstModifyCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmAnswerRemoteModifyCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* 清空 */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstModifyCnf), 0, IMSA_GET_MSG_LENGTH(pstModifyCnf));

    /* 填写消息头 */
    IMSA_WRITE_SPM_MSG_HEAD(pstModifyCnf, ID_IMSA_SPM_CALL_ANSWER_REMOTE_MODIFY_CNF);

    pstModifyCnf->usClientId = usClientId;
    pstModifyCnf->ucOpId     = (MN_OPERATION_ID_T)ulOpId;
    pstModifyCnf->enCause    = ulResult;
    pstModifyCnf->callId     = ulCallId;

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstModifyCnf);

    return VOS_TRUE;
}




/*****************************************************************************
 Function Name  : IMSA_CallProcSpmMsgInviteNewPtpt
 Description    : SPM下发的拉会议第三方请求处理
 Input          : pstAppMsg      SPM下发的命令及参数
 Output         : VOS_VOID
 Return Value   : 处理结果

 History        :
      1.xiongxianghui00253310      2014-07-10  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_CallProcSpmMsgInviteNewPtpt(VOS_VOID *pMsg)
{
    VOS_UINT32 ulResult     = VOS_FALSE;
    VOS_UINT32 ulCheckRst   = VOS_FALSE;
    IMSA_CALL_ENTITY_STRU  *pstCallEntity   = VOS_NULL_PTR;
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();
    SPM_IMSA_CALL_INVITE_NEW_PTPT_REQ_STRU *pstAppMsg   = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgInviteNewPtpt is entered!");

    pstAppMsg = (SPM_IMSA_CALL_INVITE_NEW_PTPT_REQ_STRU*) pMsg;

    /* 拉会议第三方可行性检查 */
    ulCheckRst = IMSA_CallInviteNewPtptAvailableCheck();
    if (VOS_FALSE == ulCheckRst)
    {
        IMSA_INFO_LOG1("IMSA_CallProcSpmMsgInviteNewPtpt: IMSA_CallInviteNewPtptAvailableCheck result", ulCheckRst);

        (VOS_VOID)IMSA_CallSendSpmInviteNewPtptCnfMsg(pstAppMsg->usClientId,
                                                      pstAppMsg->ucOpId,
                                                      IMSA_CALL_INVALID_ID,
                                                      TAF_CS_CAUSE_IMSA_ERROR);

        return ulResult;
    }

    pstCallEntity = IMSA_CallEntityAlloc();
    if (VOS_NULL_PTR != pstCallEntity)
    {
        /* 保存CLIENT ID和OPID */
        pstCallEntity->usClientId = pstAppMsg->usClientId;
        pstCallEntity->ulOpId = pstAppMsg->ucOpId;

        ulResult = IMSA_CallSendImsMsgInviteNewPtpt(IMSA_CALL_INVALID_ID, pstAppMsg);
        if (VOS_TRUE == ulResult)
        {
            #if 0
            /* 保存呼叫Id */
            pstCallEntity->ucId = (VOS_UINT8)IMSA_CALL_INVALID_ID;
            #endif

            /* 保存触发此呼叫对应的SPM命令，如果呼叫被拒绝且需要紧急注册后再重试时使用 */
            IMSA_MEM_CPY(   &pstCallEntity->stSpmMsg,
                            &pstCallCtx->stSpmMsg,
                            sizeof(IMSA_CALL_SPM_MSG_STRU));

            /* 启动保护定时器 */
            IMSA_StartTimer(&pstCallCtx->stProctectTimer);
        }
        else
        {
            IMSA_ERR_LOG("IMSA_CallProcSpmMsgInviteNewPtpt: send IMS msg failure");

            IMSA_CallEntityFree(pstCallEntity);
        }
    }
    else
    {
        /* 分配资源失败，拒绝该请求 */
        (VOS_VOID)IMSA_CallSendSpmInviteNewPtptCnfMsg(pstAppMsg->usClientId,
                                                      pstAppMsg->ucOpId,
                                                      IMSA_CALL_INVALID_ID,
                                                      TAF_CS_CAUSE_IMSA_ALLOC_ENTITY_FAIL);
    }

    return ulResult;
}
/* xiongxianghui00253310 add for conference 20140210 end */

VOS_UINT32 IMSA_InsertSrvccBuff
(
    const VOS_UINT8                    *pucData,
    VOS_UINT32                          ulDataLen
)
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_InsertSrvccBuff  entered!");

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    if (ulDataLen > sizeof(IMSA_SRVCC_BUFF_MSG_UNION))
    {
        IMSA_WARN_LOG("IMSA_InsertSrvccBuff:data len illegal");

        return IMSA_FAIL;
    }

    if (IMSA_CALL_MSG_SYNC_MAX_NUM <= pstSrvccBuff->ucBuffMsgNum)
    {
        IMSA_WARN_LOG("IMSA_InsertSrvccBuff:buffer full");

        return IMSA_FAIL;
    }

    IMSA_MEM_CPY(   &pstSrvccBuff->aucBuffMsgArray[pstSrvccBuff->ucBuffMsgNum],
                    pucData,
                    ulDataLen);

    pstSrvccBuff->ucBuffMsgNum++;

    return IMSA_SUCC;
}


VOS_VOID IMSA_CallOrigReqSrvccingProc
(
    const SPM_IMSA_CALL_ORIG_REQ_STRU  *pstCallOrigReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstCallOrigReq,
                                            sizeof(SPM_IMSA_CALL_ORIG_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_CallOrigReqSrvccingProc:buffer fail");

        (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstCallOrigReq->usClientId,
                                             pstCallOrigReq->ucOpId,
                                             IMSA_CALL_INVALID_ID,
                                             TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_CallOrigReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_CallSupsCmdReqSrvccingProc
(
    const SPM_IMSA_CALL_SUPS_CMD_REQ_STRU  *pstCallSupsCmdReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstCallSupsCmdReq,
                                            sizeof(SPM_IMSA_CALL_SUPS_CMD_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_CallSupsCmdReqSrvccingProc:buffer fail");

        (VOS_VOID)IMSA_CallSendSpmSupsCnfMsg(   pstCallSupsCmdReq->usClientId,
                                                pstCallSupsCmdReq->ucOpId,
                                                pstCallSupsCmdReq->stCallMgmtCmd.callId,
                                                TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_CallSupsCmdReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_CallStartDtmfReqSrvccingProc
(
    const SPM_IMSA_CALL_START_DTMF_REQ_STRU  *pstCallStartDtmfReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstCallStartDtmfReq,
                                            sizeof(SPM_IMSA_CALL_START_DTMF_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_CallStartDtmfReqSrvccingProc:buffer fail");

        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstCallStartDtmfReq->usClientId,
                                                    pstCallStartDtmfReq->ucOpId,
                                                    pstCallStartDtmfReq->stDtmf.CallId,
                                                    TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_CallStartDtmfReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_CallStopDtmfReqSrvccingProc
(
    const SPM_IMSA_CALL_STOP_DTMF_REQ_STRU  *pstCallStopDtmfReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstCallStopDtmfReq,
                                            sizeof(SPM_IMSA_CALL_STOP_DTMF_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_CallStopDtmfReqSrvccingProc:buffer fail");

        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstCallStopDtmfReq->usClientId,
                                                    pstCallStopDtmfReq->ucOpId,
                                                    pstCallStopDtmfReq->stDtmf.CallId,
                                                    TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_CallStopDtmfReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_UssdProcUssdReqSrvccingProc
(
    const SPM_IMSA_PROCESS_USSD_REQ_STRU  *pstProcUssdReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstProcUssdReq,
                                            sizeof(SPM_IMSA_PROCESS_USSD_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_UssdProcUssdReqSrvccingProc:buffer fail");
        (VOS_VOID)IMSA_SendSpmUssdErrorEvt( pstProcUssdReq->usClientId,
                                            pstProcUssdReq->ucOpId,
                                            TAF_ERR_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_UssdProcUssdReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_ClearSrvccBuff( VOS_VOID )
{
    IMSA_MEM_SET(   IMSA_GetSrvccBuffAddress(),
                    0,
                    sizeof(IMSA_SRVCC_BUFFER_STRU));
}


VOS_UINT32 IMSA_SrvccingBufferMsgProc( const VOS_VOID *pBuffMsg )
{
    /* 定义消息头指针*/
    PS_MSG_HEADER_STRU          *pstHeader = VOS_NULL_PTR;

    /* 获取消息头指针*/
    pstHeader = (PS_MSG_HEADER_STRU *) pBuffMsg;

    /* 如果不在SRVCC过程中，则直接退出 */
    if (IMSA_TRUE != IMSA_CallGetSrvccFlag())
    {
        return IMSA_MSG_NOT_PROC;
    }

    switch (pstHeader->ulMsgName)
    {
        case ID_SPM_IMSA_CALL_ORIG_REQ:

            IMSA_CallOrigReqSrvccingProc(pBuffMsg);
            break;

        case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:

            IMSA_CallSupsCmdReqSrvccingProc(pBuffMsg);
            break;

        case ID_SPM_IMSA_CALL_START_DTMF_REQ:

            IMSA_CallStartDtmfReqSrvccingProc(pBuffMsg);
            break;

        case ID_SPM_IMSA_CALL_STOP_DTMF_REQ:

            IMSA_CallStopDtmfReqSrvccingProc(pBuffMsg);
            break;

        case ID_SPM_IMSA_PROCESS_USSD_REQ:

            IMSA_UssdProcUssdReqSrvccingProc(pBuffMsg);
            break;

        default:

            return IMSA_MSG_NOT_PROC;

    }

    return IMSA_MSG_PROCED;
}
VOS_VOID IMSA_CallSendCallMsgSyncInd
(
    VOS_UINT32                          ulSynMsgNum,
    const VOS_UINT32                   *pulSynMsgIndexArray
)
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff    = VOS_NULL_PTR;
    IMSA_CALL_MSG_SYNC_IND_STRU        *pstMsgSyncInd   = VOS_NULL_PTR;
    VOS_UINT32                          i               = IMSA_NULL;

    if ((ulSynMsgNum == 0) || (ulSynMsgNum > IMSA_CALL_MSG_SYNC_MAX_NUM)
        || (VOS_NULL_PTR == pulSynMsgIndexArray))
    {
        return ;
    }

    IMSA_INFO_LOG("IMSA_CallSendCallMsgSyncInd is entered!");

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    pstMsgSyncInd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_CALL_MSG_SYNC_IND_STRU));

    if (VOS_NULL_PTR == pstMsgSyncInd)
    {
        IMSA_ERR_LOG("IMSA_CallSendCallMsgSyncInd: alloc memory fail");
        return ;
    }

    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgSyncInd), 0, IMSA_GET_MSG_LENGTH(pstMsgSyncInd));

    for (i = 0; i < ulSynMsgNum; i++)
    {
        IMSA_MEM_CPY(   &pstMsgSyncInd->astMsgArray[pstMsgSyncInd->ucMsgNum],
                        &pstSrvccBuff->aucBuffMsgArray[pulSynMsgIndexArray[i]],
                        sizeof(IMSA_CALL_MSG_UNION));

        pstMsgSyncInd->ucMsgNum++;
    }

    /* 填写消息头 */
    IMSA_WRITE_CALL_MSG_HEAD(pstMsgSyncInd, ID_IMSA_CALL_MSG_SYNC_IND);

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstMsgSyncInd);
}


/*****************************************************************************
 Function Name  : IMSA_CallSendCcwaCapInfo
 Description    : IMSA向CALL模块通告CCWA配置，
 Input          : ucCcwacap---------------CCWA配置信息
                  0:不支持CCWA
                  1:支持CCWA
 Output         : None
 Return Value   :
                  失败:VOS_FALSE ;
                  成功:VOS_TRUE
 History        :
      1.jiaguocai 00355737     2015-08-27  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_CallSendCcwaCapInfo(VOS_UINT8 ucCcwacap)
{
    IMSA_CALL_CCWA_CAP_NOTIFY_STRU  *pstCcwaCapNotify = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendCcwaCapInfo is entered!");

    pstCcwaCapNotify = (VOS_VOID *)IMSA_ALLOC_MSG(sizeof(IMSA_CALL_CCWA_CAP_NOTIFY_STRU));

    if (VOS_NULL_PTR == pstCcwaCapNotify)
    {
        IMSA_ERR_LOG("IMSA_CallSendCcwaCapInfo: alloc memory fail");
        return ;
    }

    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstCcwaCapNotify), 0, IMSA_GET_MSG_LENGTH(pstCcwaCapNotify));

    /* 填写消息头 */
    IMSA_WRITE_CALL_MSG_HEAD(pstCcwaCapNotify, ID_IMSA_CALL_CCWA_CAP_NOTIFY);

    pstCcwaCapNotify->ucCcwaCap = ucCcwacap;

    /* 调用消息发送函数 */
    IMSA_SND_MSG(pstCcwaCapNotify);
}




VOS_VOID IMSA_SrvccSuccBuffProc( VOS_VOID )
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff    = VOS_NULL_PTR;
    PS_MSG_HEADER_STRU                 *pstHeader       = VOS_NULL_PTR;
    SPM_IMSA_PROCESS_USSD_REQ_STRU     *pstUssdReq      = VOS_NULL_PTR;
    VOS_UINT32                          ulSynMsgNum     = IMSA_NULL;
    VOS_UINT32                          aulSynMsgIndexArray[IMSA_CALL_MSG_SYNC_MAX_NUM] = {0};
    VOS_UINT32                          i               = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_SrvccSuccBuffProc  entered!");

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    if (pstSrvccBuff->ucBuffMsgNum == 0)
    {
        IMSA_INFO_LOG("IMSA_SrvccSuccBuffProc:no buffer msg");
        return ;
    }

    for (i = 0; i < pstSrvccBuff->ucBuffMsgNum; i++)
    {
        pstHeader = (PS_MSG_HEADER_STRU *)(&pstSrvccBuff->aucBuffMsgArray[i]);

        if (pstHeader->ulMsgName != ID_SPM_IMSA_PROCESS_USSD_REQ)
        {
            aulSynMsgIndexArray[ulSynMsgNum] = i;
            ulSynMsgNum++;
            continue;
        }

        /* 将USSD之前的缓存消息发给CALL模块 */
        IMSA_CallSendCallMsgSyncInd(ulSynMsgNum, aulSynMsgIndexArray);
        ulSynMsgNum = 0;

        /* 回复失败，原因值为SRVCC_SUCC，触发SPM到CS域重播 */
        pstUssdReq = (SPM_IMSA_PROCESS_USSD_REQ_STRU*)pstHeader;
        (VOS_VOID)IMSA_SendSpmUssdErrorEvt( pstUssdReq->usClientId,
                                            pstUssdReq->ucOpId,
                                            TAF_ERR_IMSA_SRVCCING_BUFF_MSG);
    }

    IMSA_CallSendCallMsgSyncInd(ulSynMsgNum, aulSynMsgIndexArray);
    /* ulSynMsgNum = 0; */

    /* 清缓存 */
    IMSA_ClearSrvccBuff();
}


VOS_VOID IMSA_SrvccFailBuffProc( VOS_VOID )
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff    = VOS_NULL_PTR;
    PS_MSG_HEADER_STRU                 *pstHeader       = VOS_NULL_PTR;
    VOS_UINT32                          i               = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_SrvccFailBuffProc  entered!");

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    if (pstSrvccBuff->ucBuffMsgNum == 0)
    {
        IMSA_INFO_LOG("IMSA_SrvccFailBuffProc:no buffer msg");
        return ;
    }

    for (i = 0; i < pstSrvccBuff->ucBuffMsgNum; i++)
    {
         pstHeader = (PS_MSG_HEADER_STRU *)(&pstSrvccBuff->aucBuffMsgArray[i]);

         switch (pstHeader->ulMsgName)
         {
            case ID_SPM_IMSA_CALL_ORIG_REQ:

                (VOS_VOID)IMSA_CallProcSpmMsgOrig((SPM_IMSA_CALL_ORIG_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:

                (VOS_VOID)IMSA_CallProcSpmMsgSups((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            case ID_SPM_IMSA_CALL_START_DTMF_REQ:

                IMSA_CallProcSpmMsgStartDtmf((SPM_IMSA_CALL_START_DTMF_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            case ID_SPM_IMSA_CALL_STOP_DTMF_REQ:

                IMSA_CallProcSpmMsgStopDtmf((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            case ID_SPM_IMSA_PROCESS_USSD_REQ:

                IMSA_SsProcSpmMsgProcessUssdReq((SPM_IMSA_PROCESS_USSD_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            default:

                IMSA_WARN_LOG("IMSA_SrvccFailBuffProc:illegal buffer msg type!");
                break;
         }
    }

    /* 清缓存 */
    IMSA_ClearSrvccBuff();
}
VOS_VOID IMSA_SrvccAbormalClearBuff
(
    IMSA_SRVCC_ABNORMAL_TYPE_ENUM_UINT32    enAbnormalType
)
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff    = VOS_NULL_PTR;
    PS_MSG_HEADER_STRU                 *pstHeader       = VOS_NULL_PTR;
    VOS_UINT32                          i               = IMSA_NULL;
    TAF_CS_CAUSE_ENUM_UINT32            ulTafCsCause    = TAF_CS_CAUSE_IMSA_SRVCC_ABNORMAL;
    VOS_UINT16                          usUssdError     = TAF_ERR_IMSA_SRVCC_ABNORMAL;

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    if (pstSrvccBuff->ucBuffMsgNum == 0)
    {
        IMSA_INFO_LOG("IMSA_SrvccAbormalClearBuff:no buffer msg");
        return ;
    }

    IMSA_INFO_LOG("IMSA_SrvccAbormalClearBuff  entered!");

    if (IMSA_SRVCC_ABNORMAL_DEREG_REQ == enAbnormalType)
    {
        ulTafCsCause    = TAF_CS_CAUSE_IMSA_SRVCC_ABNORMAL_DEREG;
        usUssdError     = TAF_ERR_IMSA_SRVCC_ABNORMAL_DEREG;
    }

    for (i = 0; i < pstSrvccBuff->ucBuffMsgNum; i++)
    {
         pstHeader = (PS_MSG_HEADER_STRU *)(&pstSrvccBuff->aucBuffMsgArray[i]);

         switch (pstHeader->ulMsgName)
         {
            case ID_SPM_IMSA_CALL_ORIG_REQ:

                (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(((SPM_IMSA_CALL_ORIG_REQ_STRU*)pstHeader)->usClientId,
                                                    ((SPM_IMSA_CALL_ORIG_REQ_STRU*)pstHeader)->ucOpId,
                                                    IMSA_CALL_INVALID_ID,
                                                    ulTafCsCause);
                break;

            case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:

                (VOS_VOID)IMSA_CallSendSpmSupsCnfMsg(((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU*)pstHeader)->usClientId,
                                                    ((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU*)pstHeader)->ucOpId,
                                                    ((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU*)pstHeader)->stCallMgmtCmd.callId,
                                                    ulTafCsCause);
                break;

            case ID_SPM_IMSA_CALL_START_DTMF_REQ:

                (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  ((SPM_IMSA_CALL_START_DTMF_REQ_STRU*)pstHeader)->usClientId,
                                                            ((SPM_IMSA_CALL_START_DTMF_REQ_STRU*)pstHeader)->ucOpId,
                                                            ((SPM_IMSA_CALL_START_DTMF_REQ_STRU*)pstHeader)->stDtmf.CallId,
                                                            ulTafCsCause);
                break;

            case ID_SPM_IMSA_CALL_STOP_DTMF_REQ:

                (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   ((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU*)pstHeader)->usClientId,
                                                            ((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU*)pstHeader)->ucOpId,
                                                            ((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU*)pstHeader)->stDtmf.CallId,
                                                            ulTafCsCause);
                break;

            case ID_SPM_IMSA_PROCESS_USSD_REQ:

                /* 回复失败，原因值为SRVICE NOT AVAILABLE */
                (VOS_VOID)IMSA_SendSpmUssdErrorEvt( ((SPM_IMSA_PROCESS_USSD_REQ_STRU*)pstHeader)->usClientId,
                                                    ((SPM_IMSA_PROCESS_USSD_REQ_STRU*)pstHeader)->ucOpId,
                                                    usUssdError);
                break;

            default:

                IMSA_WARN_LOG("IMSA_SrvccAbormalClearBuff:illegal buffer msg type!");
                break;
         }
    }

    /* 清缓存 */
    IMSA_ClearSrvccBuff();
}

VOS_UINT32 IMSA_CallProcSpmMsgModifyReq
(
    VOS_VOID *pMsg
)
{
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();
    VOS_UINT32 ulResult = VOS_TRUE;
    SPM_IMSA_CALL_MODIFY_REQ_STRU *pstAppMsg    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgModifyReq is entered!");

    pstAppMsg = (SPM_IMSA_CALL_MODIFY_REQ_STRU*)pMsg;


    ulResult = IMSA_CallSendImsMsgModify(pstAppMsg);

    if (VOS_TRUE == ulResult)
    {
        /* 启动保护定时器 */
        IMSA_StartTimer(&pstCallCtx->stProctectTimer);
    }
    else
    {
        IMSA_ERR_LOG("IMSA_CallProcSpmMsgModifyReq: error occurs, should return failure to SPM");

        /* 需要通知SPM 切换流程失败 */
        IMSA_CallSendSpmModifyCnfMsg(pstAppMsg->usClientId,
                                     pstAppMsg->ucOpId,
                                     pstAppMsg->callId,
                                     TAF_CS_CAUSE_IMSA_ERROR);
        ulResult = VOS_FALSE;

    }

    return ulResult;
}

VOS_UINT32 IMSA_CallProcSpmMsgAnswerRemoteModifyReq
(
    VOS_VOID *pMsg
)
{
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();
    VOS_UINT32 ulResult = VOS_TRUE;
    SPM_IMSA_CALL_ANSWER_REMOTE_MODIFY_REQ_STRU *pstAppMsg    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgAnswerRemoteModifyReq is entered!");

    pstAppMsg = (SPM_IMSA_CALL_ANSWER_REMOTE_MODIFY_REQ_STRU*)pMsg;


    ulResult = IMSA_CallSendImsMsgAnswerRemoteModify(pstAppMsg);

    if (VOS_TRUE == ulResult)
    {
        /* 启动保护定时器 */
        IMSA_StartTimer(&pstCallCtx->stProctectTimer);
    }
    else
    {
        IMSA_ERR_LOG("IMSA_CallProcSpmMsgAnswerRemoteModifyReq: error occurs, should return failure to SPM");

        /* 需要通知SPM 切换流程失败 */
        IMSA_CallSendSpmAnswerRemoteModifyCnfMsg(pstAppMsg->usClientId,
                                                 pstAppMsg->ucOpId,
                                                 pstAppMsg->callId,
                                                 TAF_CS_CAUSE_IMSA_ERROR);
        ulResult = VOS_FALSE;

    }

    return ulResult;
}




VOS_VOID IMSA_ProcSpmCallMsg(const VOS_VOID *pRcvMsg)
{
    /* 定义消息头指针*/
    PS_MSG_HEADER_STRU                 *pstHeader = VOS_NULL_PTR;
    VOS_UINT32                           ulIndex = 0;
    IMSA_SPM_CALL_MSG_ACTION_FUN        pActFun;

    IMSA_INFO_LOG("IMSA_ProcSpmCallMsg is entered!");

    /* 获取消息头指针*/
    pstHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    if (VOS_NULL_PTR == pstHeader)
    {
        IMSA_ERR_LOG("IMSA_ProcSpmCallMsg: NULL message");
        return;
    }

    IMSA_INFO_LOG1("IMSA_ProcSpmCallMsg: msg id", pstHeader->ulMsgName);

    if (IMSA_MSG_PROCED == IMSA_SrvccingBufferMsgProc(pRcvMsg))
    {
        IMSA_INFO_LOG("IMSA_ProcSpmCallMsg: Srvccing buffer!");
        return ;
    }

    /*根据回复结果,查询处理函数 ,并做相应处理*/
    for (ulIndex = 0; ulIndex < g_ulImsaSpmCallMsgActTblLen; ulIndex++)
    {
        /* 通过MSG ID查找处理函数 */
        if(pstHeader->ulMsgName == g_stImsaSpmCallMsgActTbl[ulIndex].enMsgName)
        {
            pActFun = g_stImsaSpmCallMsgActTbl[ulIndex].pfActionFun;

            /* 有处理函数 */
            if (VOS_NULL_PTR != pActFun)
            {
                (VOS_VOID)(pActFun)((VOS_VOID*)pRcvMsg);
            }
        }
    }
    return;
}
/*lint +e960*/
/*lint +e961*/


#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcSpmMsg.c */



