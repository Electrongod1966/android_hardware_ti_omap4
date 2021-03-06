/*
 *  Copyright 2001-2009 Texas Instruments - http://www.ti.com/
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*============================================================================
 *  @file   dehApp.c
 *
 *  @brief  Sample application for DEH module between MPU & IPU
 *
 *  ============================================================================
 */


/* Standard headers */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <Std.h>

/* OSAL & Utils headers */
#include <Trace.h>
#include <OsalPrint.h>
#include <Memory.h>
#include <String.h>

/* Module level headers */
#include <IpcUsr.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/HeapBufMP.h>
#include <ti/ipc/NameServer.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/ListMP.h>
#include <ti/ipc/Notify.h>

/* Application header */
#include "dehApp_config.h"


#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

typedef struct MsgInfo {
    MessageQ_MsgHeader  header;
    UInt32              cmd;
    Int32               arg [MAX_NUM_ARGS];
} MsgInfo;

/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Name of the SysM3 baseImage to be used for sample execution with
 *          SysM3
 */
#define DEH_SYSM3ONLY_IMAGE "./Deh_MPUSYS_Test_Core0.xem3"

/*!
 *  @brief  Name of the SysM3 baseImage to be used for sample execution with
 *          AppM3
 */
#define DEH_SYSM3_IMAGE     "./Notify_MPUSYS_reroute_Test_Core0.xem3"

/*!
 *  @brief  Name of the AppM3 baseImage to be used for sample execution with
 *          AppM3
 */
#define DEH_APPM3_IMAGE     "./Deh_MPUAPP_Test_Core1.xem3"

/*!
 *  @brief  Number of transfers to be tested.
 */
#define  DEH_NUM_TRANSFERS  2

/** ============================================================================
 *  Globals
 *  ============================================================================
 */
MessageQ_Handle                deh_messageQ;
HeapBufMP_Handle               deh_heapHandle;
MessageQ_QueueId               deh_queueId = MessageQ_INVALIDMESSAGEQ;
UInt16                         deh_procId;
UInt16                         deh_procId1;
UInt32                         deh_curShAddr;
ProcMgr_Handle                 deh_procMgrHandle;
ProcMgr_Handle                 deh_procMgrHandle1;
SizeT                          deh_heapSize         = 0;
Ptr                            deh_ptr              = NULL;


/** ============================================================================
 *  Functions
 *  ============================================================================
 */
/*!
 *  @brief  Function to execute the startup for DEH sample application
 *
 *  @sa
 */
Int
deh_startup (Int testNo)
{
    Int32                          status           = 0;
    Ipc_Config                     config;
    UInt32                         srCount;
    SharedRegion_Entry             srEntry;
    Int                            i;
#if !defined (SYSLINK_USE_DAEMON)
    HeapBufMP_Params               heapbufmpParams;
    IHeap_Handle                   srHeap           = NULL;
    UInt32                         entryPoint       = 0;
    ProcMgr_StartParams            startParams;
#if defined(SYSLINK_USE_LOADER)
    Char                         * imageName;
    UInt32                         fileId;
#endif /* if defined(SYSLINK_USE_LOADER) */
#endif /* if !defined(SYSLINK_USE_DAEMON) */
    ProcMgr_AttachParams           attachParams;
    ProcMgr_State                  state;

    Osal_printf ("Entered deh_startup\n");

    Ipc_getConfig (&config);
    status = Ipc_setup (&config);
    if (status < 0) {
        Osal_printf ("Error in Ipc_setup [0x%x]\n", status);
    }

    /* Open a handle to the SysM3 ProcMgr instance. */
    deh_procId = MultiProc_getId ("SysM3");
    status = ProcMgr_open (&deh_procMgrHandle, deh_procId);
    if (status < 0) {
        Osal_printf ("Error in ProcMgr_open (SysM3) [0x%x]\n", status);
    }
    else {
        Osal_printf ("ProcMgr_open (SysM3) Status [0x%x]\n", status);
        ProcMgr_getAttachParams (NULL, &attachParams);
        /* Default params will be used if NULL is passed. */
        status = ProcMgr_attach (deh_procMgrHandle, &attachParams);
        if (status < 0) {
            Osal_printf ("ProcMgr_attach (SysM3) failed [0x%x]\n", status);
        }
        else {
            Osal_printf ("ProcMgr_attach (SysM3) status: [0x%x]\n", status);
            state = ProcMgr_getState (deh_procMgrHandle);
            Osal_printf ("After attach: ProcMgr_getState (SysM3)\n"
                         "    state [0x%x]\n", status);
        }
    }

    /* Open a handle to the AppM3 ProcMgr instance. */
    if ((status >= 0) && (testNo == 2)) {
        deh_procId1 = MultiProc_getId ("AppM3");
        status = ProcMgr_open (&deh_procMgrHandle1, deh_procId1);
        if (status < 0) {
            Osal_printf ("Error in ProcMgr_open (AppM3) [0x%x]\n", status);
        }
        else {
            Osal_printf ("ProcMgr_open (AppM3) Status [0x%x]\n", status);
            ProcMgr_getAttachParams (NULL, &attachParams);
            /* Default params will be used if NULL is passed. */
            status = ProcMgr_attach (deh_procMgrHandle1, &attachParams);
            if (status < 0) {
                Osal_printf ("ProcMgr_attach (AppM3) failed [0x%x]\n", status);
            }
            else {
                Osal_printf ("ProcMgr_attach(AppM3) status: [0x%x]\n", status);
                state = ProcMgr_getState (deh_procMgrHandle1);
                Osal_printf ("After attach: ProcMgr_getState (AppM3)\n"
                             "    state [0x%x]\n", state);
            }
        }
    }

#if !defined(SYSLINK_USE_DAEMON) /* Daemon sets this up */
#ifdef SYSLINK_USE_LOADER
    if (status >= 0) {
        if (testNo == 1)
            imageName = DEH_SYSM3ONLY_IMAGE;
        else if (testNo == 2)
            imageName = DEH_SYSM3_IMAGE;

        Osal_printf ("Loading image (%s) onto Ducati with ProcId %d\n",
                        imageName, deh_procId);
        status = ProcMgr_load (deh_procMgrHandle, imageName, 2,
                                (String *) &imageName, &entryPoint, &fileId,
                                deh_procId);
        if (status < 0) {
            Osal_printf ("Error in ProcMgr_load (SysM3) image: [0x%x]\n", status);
        }
        else {
            Osal_printf ("ProcMgr_load (SysM3) Status [0x%x]\n", status);
        }
    }
#endif /* defined(SYSLINK_USE_LOADER) */
    if (status >= 0) {
        startParams.proc_id = deh_procId;
        status = ProcMgr_start (deh_procMgrHandle, entryPoint,
                                &startParams);
        if (status < 0) {
            Osal_printf ("Error in ProcMgr_start (SysM3) [0x%x]\n", status);
        }
        else {
           Osal_printf ("ProcMgr_start (SysM3) Status [0x%x]\n", status);
        }
    }

    if ((status >= 0) && (testNo == 2)) {
#if defined(SYSLINK_USE_LOADER)
        imageName = DEH_APPM3_IMAGE;
        status = ProcMgr_load (deh_procMgrHandle1, imageName, 2,
                                &imageName, &entryPoint, &fileId,
                                deh_procId1);
        if (status < 0) {
            Osal_printf ("Error in ProcMgr_load (AppM3) image: 0x%x]\n", status);
        }
        else {
            Osal_printf ("ProcMgr_load (AppM3) Status [0x%x]\n", status);
        }
#endif /* defined(SYSLINK_USE_LOADER) */
        startParams.proc_id = deh_procId1;
        status = ProcMgr_start (deh_procMgrHandle1, entryPoint,
                                    &startParams);
        if (status < 0) {
            Osal_printf ("Error in ProcMgr_start (AppM3) [0x%x]\n", status);
        }
        else {
            Osal_printf ("ProcMgr_start (AppM3) Status [0x%x]\n", status);
        }
    }
#endif /* !defined(SYSLINK_USE_DAEMON) */

    srCount = SharedRegion_getNumRegions();
    Osal_printf ("SharedRegion_getNumRegions = %d\n", srCount);
    for (i = 0; i < srCount; i++) {
        status = SharedRegion_getEntry (i, &srEntry);
        Osal_printf ("SharedRegion_entry #%d: base = 0x%x len = 0x%x "
                        "ownerProcId = %d isValid = %d cacheEnable = %d "
                        "cacheLineSize = 0x%x createHeap = %d name = %s\n",
                        i, srEntry.base, srEntry.len, srEntry.ownerProcId,
                        (Int)srEntry.isValid, (Int)srEntry.cacheEnable,
                        srEntry.cacheLineSize, (Int)srEntry.createHeap,
                        srEntry.name);
    }

#if !defined(SYSLINK_USE_DAEMON) /* Daemon sets this up */
    /* Create Heap and register it with MessageQ */
    if (status >= 0) {
        HeapBufMP_Params_init (&heapbufmpParams);
        heapbufmpParams.sharedAddr = NULL;
        heapbufmpParams.align      = 128;
        heapbufmpParams.numBlocks  = 4;
        heapbufmpParams.blockSize  = MSGSIZE;
        deh_heapSize = HeapBufMP_sharedMemReq (&heapbufmpParams);
        Osal_printf ("heapSize = 0x%x\n", deh_heapSize);

        srHeap = SharedRegion_getHeap (0);
        if (srHeap == NULL) {
            status = MessageQ_E_FAIL;
            Osal_printf ("SharedRegion_getHeap failed for %d processor."
                         " srHeap: [0x%x]\n",
                         deh_procId,
                         srHeap);
        }
        else {
            Osal_printf ("Before Memory_alloc = 0x%x\n", srHeap);
            deh_ptr = Memory_alloc (srHeap,
                                deh_heapSize,
                                0);
            if (deh_ptr == NULL) {
                status = MEMORYOS_E_MEMORY;
                Osal_printf ("Memory_alloc failed for %d processor."
                             " ptr: [0x%x]\n",
                             deh_procId,
                             deh_ptr);
            }
            else {
                heapbufmpParams.name           = "Heap0";
                heapbufmpParams.sharedAddr     = deh_ptr;
                Osal_printf ("Before HeapBufMP_Create: [0x%x]\n",
                                deh_ptr);
                deh_heapHandle = HeapBufMP_create (&heapbufmpParams);
                if (deh_heapHandle == NULL) {
                    status = HeapBufMP_E_FAIL;
                    Osal_printf ("HeapBufMP_create failed for %d processor."
                                 " Handle: [0x%x]\n",
                                 deh_procId,
                                 deh_heapHandle);
                }
                else {
                    /* Register this heap with MessageQ */
                    status = MessageQ_registerHeap (deh_heapHandle,
                                                    HEAPID);
                    if (status < 0) {
                        Osal_printf ("MessageQ_registerHeap failed!\n");
                    }
                }
            }
        }
    }
#endif /* !defined(SYSLINK_USE_DAEMON) */

    Osal_printf ("Leaving deh_startup: status = 0x%x\n", status);

    return (status);
}


/*!
 *  @brief  Function to execute the DEH sample application
 *
 *  @sa
 */
Int
deh_execute (Int testNo, Int cmd , Int* arg)
{
    Int32                    status     = 0;
    MessageQ_Msg             msg        = NULL;
    MessageQ_Params          msgParams;
    UInt16                   i;
    Char                   * msgQName;
    MsgInfo*                 dehMsg;

    Osal_printf ("Entered deh_execute\n");

    /* Create the Message Queue. */
    MessageQ_Params_init (&msgParams);
    deh_messageQ = MessageQ_create (ARM_MESSAGEQNAME, &msgParams);
    if (deh_messageQ == NULL) {
        Osal_printf ("Error in MessageQ_create\n");
    }
    else {
        Osal_printf ("MessageQ_create handle [0x%x]\n",
                     deh_messageQ);
    }

    /* Assign the MessageQ Name being opened */
    switch (testNo) {
        case 2:
            msgQName = DUCATI_CORE1_MESSAGEQNAME;
            break;

        case 1:
        default:
            msgQName = DUCATI_CORE0_MESSAGEQNAME;
            break;
    }

    if (status >=0) {
        do {
            status = MessageQ_open (msgQName, &deh_queueId);
        } while (status == MessageQ_E_NOTFOUND);
        if (status < 0) {
            Osal_printf ("Error in MessageQ_open [0x%x]\n", status);
        }
        else {
            Osal_printf ("MessageQ_open Status [0x%x]\n", status);
            Osal_printf ("deh_queueId  [0x%x]\n", deh_queueId);
        }
    }

    if (status >= 0) {
        Osal_printf ("\nExchanging messages with remote processor\n");
        for (i = 0 ; i < DEH_NUM_TRANSFERS ; i++) {
            /* Allocate message. */
            msg = MessageQ_alloc (HEAPID, sizeof (MsgInfo));
            if (msg == NULL) {
                Osal_printf ("Error in MessageQ_alloc\n");
                break;
            }
            else {
                Osal_printf ("MessageQ_alloc msg [0x%x]\n", msg);
                dehMsg = (MsgInfo*)msg;
                dehMsg->cmd = cmd;
                Memory_copy ((Ptr)&dehMsg->arg, (Ptr)arg, MAX_NUM_ARGS * sizeof(Int32));
            }

            MessageQ_setMsgId (msg, (i % DEH_NUM_TRANSFERS));

            /* Have the DSP reply to this message queue */
            MessageQ_setReplyQueue (deh_messageQ, msg);

            status = MessageQ_put (deh_queueId, msg);
            if (status < 0) {
                Osal_printf ("Error in MessageQ_put [0x%x]\n",
                             status);
                break;
            }
            else {
                Osal_printf ("MessageQ_put #%d Status [0x%x]\n", i, status);
            }

            status = MessageQ_get(deh_messageQ, &msg, MessageQ_FOREVER);
            if (status < 0) {
                Osal_printf ("Error in MessageQ_get\n");
                break;
            }
            else {
                Osal_printf ("MessageQ_get #%d Msg = 0x%x\n", i, msg);

                /* Validate the returned message. */
                if (msg != NULL) {
                    if (MessageQ_getMsgId (msg) !=
                                    ((i % DEH_NUM_TRANSFERS) + 1)) {
                        Osal_printf ("Data integrity failure!\n"
                                     "    Expected %d\n"
                                     "    Received %d\n",
                                     ((i % DEH_NUM_TRANSFERS) + 1),
                                     MessageQ_getMsgId (msg));
                        break;
                    }
                }

                status = MessageQ_free (msg);
                Osal_printf ("MessageQ_free status [0x%x]\n", status);
            }

            if ((i % 2) == 0) {
                Osal_printf ("Exchanged %d messages with remote processor\n",
                             i);
            }
        }
    }

    /* Keep the Ducati application running. */
#if !defined (SYSLINK_USE_DAEMON)
    /* Send die message */
    msg = MessageQ_alloc (HEAPID, MSGSIZE);
    if (msg == NULL) {
        Osal_printf ("MessageQ_alloc (die message) failed\n");
    }
    else {
        Osal_printf ("MessageQ_alloc (die message) msg = [0x%x]\n", msg);

        /* Send a die message */
        MessageQ_setMsgId(msg, DIEMESSAGE);

        /* Have the DSP reply to this message queue */
        MessageQ_setReplyQueue (deh_messageQ, msg);

        /* Send the message off */
        status = MessageQ_put (deh_queueId, msg);
        if (status < 0) {
            Osal_printf ("Error in MessageQ_put (die message) [0x%x]\n",
                         status);
        }
        else {
            Osal_printf ("MessageQ_put (die message) Status [0x%x]\n", status);
        }

        /* Wait for the final message. */
        status = MessageQ_get(deh_messageQ, &msg, MessageQ_FOREVER);
        if (status < 0) {
            Osal_printf ("\nError in MessageQ_get (die message)!\n");
        }
        else {
            /* Validate the returned message. */
            if (msg != NULL) {
                if (MessageQ_getMsgId (msg) == DIEMESSAGE) {
                    Osal_printf ("\nSuccessfully received die response from the "
                                 "remote processor\n");
                    Osal_printf ("Sample application successfully completed!\n");
                }
                else {
                    Osal_printf("\nUnsuccessful run of the sample "
                                "application!\n");
                }
            }
            else {
                Osal_printf("\nUnsuccessful run of the sample application msg "
                          "is NULL!\n");
            }
        }
        MessageQ_free(msg);
    }
#else
    Osal_printf ("Sample application successfully completed!\n");
#endif /* !SYSLINK_USE_DAEMON */

    /* Clean-up */
    if (deh_messageQ != NULL) {
        status = MessageQ_delete (&deh_messageQ);
        if (status < 0) {
            Osal_printf ("Error in MessageQ_delete [0x%x]\n",
                         status);
        }
        else {
            Osal_printf ("MessageQ_delete Status [0x%x]\n", status);
        }
    }

    if (deh_queueId != MessageQ_INVALIDMESSAGEQ) {
        MessageQ_close (&deh_queueId);
    }

    Osal_printf ("Leaving deh_execute\n");

    return (status);
}


/*!
 *  @brief Function to execute the shutdown for DEH sample app
 *
 *  @sa
 */
Int
deh_shutdown (Int testNo)
{
    Int32               status = 0;
#if !defined (SYSLINK_USE_DAEMON)
    ProcMgr_StopParams  stopParams;
    IHeap_Handle        srHeap = NULL;
#endif

    Osal_printf ("Entered deh_shutdown()\n");

#if !defined (SYSLINK_USE_DAEMON)
    status = MessageQ_unregisterHeap (HEAPID);
    Osal_printf ("MessageQ_unregisterHeap status: [0x%x]\n", status);

    if (deh_heapHandle) {
        status = HeapBufMP_delete (&deh_heapHandle);
        Osal_printf ("HeapBufMP_delete status: [0x%x]\n", status);
    }

    if (deh_ptr) {
        srHeap = SharedRegion_getHeap (0);
        Memory_free (srHeap, deh_ptr, deh_heapSize);
    }

    if (testNo == 2) {
        stopParams.proc_id = deh_procId1;
        status = ProcMgr_stop (deh_procMgrHandle1, &stopParams);
        Osal_printf ("ProcMgr_stop status: [0x%x]\n", status);
    }

    stopParams.proc_id = deh_procId;
    status = ProcMgr_stop (deh_procMgrHandle, &stopParams);
    Osal_printf ("ProcMgr_stop status: [0x%x]\n", status);
#endif /* !defined(SYSLINK_USE_DAEMON) */

    if (testNo == 2) {
        status =  ProcMgr_detach (deh_procMgrHandle1);
        Osal_printf ("ProcMgr_detach status [0x%x]\n", status);

        status = ProcMgr_close (&deh_procMgrHandle1);
        Osal_printf ("ProcMgr_close status: [0x%x]\n", status);
    }

    status =  ProcMgr_detach (deh_procMgrHandle);
    Osal_printf ("ProcMgr_detach status [0x%x]\n", status);

    status = ProcMgr_close (&deh_procMgrHandle);
    Osal_printf ("ProcMgr_close status: [0x%x]\n", status);

    status = Ipc_destroy ();
    Osal_printf ("Ipc_destroy status: [0x%x]\n", status);

    Osal_printf ("Leave deh_shutdown()\n");

    return (status);
}
