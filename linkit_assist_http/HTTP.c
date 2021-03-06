/*
 This sample code is in public domain.

 This sample code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* 
 This sample connects to HTTP(no secure) to retrieve index.html from labs.mediatek.com and print to vm_log.

 It calls the API vm_https_register_context_and_callback() to register the callback functions,
 then set the channel by vm_https_set_channel(), after the channel is established,
 it will send out the request by vm_https_send_request() and read the response by
 vm_https_read_content().

 You can change the url by modify macro VMHTTPS_TEST_URL.
 Before run this example, please set the APN information first by modify macros.
 */
#include <string.h>
#include "vmtype.h" 
#include "vmboard.h"
#include "vmsystem.h"
#include "vmlog.h" 
#include "vmcmd.h" 
#include "vmdcl.h"
#include "vmdcl_gpio.h"
#include "vmthread.h"

#include "ResID.h"
#include "HTTP.h"
#include "vmhttps.h"
#include "vmtimer.h"
#include "vmgsm_gprs.h"

/* Configurable macros */
#define CUST_APN    "uninet"         /* The APN of your test SIM */
#define USING_PROXY VM_TRUE         /* Whether your SIM uses proxy */
#define PROXY_ADDRESS   "10.0.0.172"    /* The proxy address */
#define PROXY_PORT  80              /* The proxy port */

#define VMHTTPS_TEST_DELAY 60000    /* 60 seconds */
#define VMHTTPS_TEST_URL "http://labs.mediatek.com"

VMUINT8 g_channel_id;
VMINT g_read_seg_num;
static void https_send_request_set_channel_rsp_cb(VMUINT32 req_id,
		VMUINT8 channel_id, VMUINT8 result) {
	VMINT ret = -1;

	ret = vm_https_send_request(0, /* Request ID */
	VM_HTTPS_METHOD_GET, /* HTTP Method Constant */
	VM_HTTPS_OPTION_NO_CACHE, /* HTTP request options */
	VM_HTTPS_DATA_TYPE_BUFFER, /* Reply type (wps_data_type_enum) */
	100, /* bytes of data to be sent in reply at a time. If data is more that this, multiple response would be there */
	(VMUINT8 *) VMHTTPS_TEST_URL, /* The request URL */
	strlen(VMHTTPS_TEST_URL), /* The request URL length */
	NULL, /* The request header */
	0, /* The request header length */
	NULL, 0);

	if (ret != 0) {
		vm_https_unset_channel(channel_id);
	}
}

static void https_unset_channel_rsp_cb(VMUINT8 channel_id, VMUINT8 result) {
	vm_log_debug("https_unset_channel_rsp_cb()");
}
static void https_send_release_all_req_rsp_cb(VMUINT8 result) {
	vm_log_debug("https_send_release_all_req_rsp_cb()");
}
static void https_send_termination_ind_cb(void) {
	vm_log_debug("https_send_termination_ind_cb()");
}
static void https_send_read_request_rsp_cb(VMUINT16 request_id, VMUINT8 result,
		VMUINT16 status, VMINT32 cause, VMUINT8 protocol,
		VMUINT32 content_length, VMBOOL more, VMUINT8 *content_type,
		VMUINT8 content_type_len, VMUINT8 *new_url, VMUINT32 new_url_len,
		VMUINT8 *reply_header, VMUINT32 reply_header_len,
		VMUINT8 *reply_segment, VMUINT32 reply_segment_len) {
	VMINT ret = -1;
	vm_log_debug("https_send_request_rsp_cb()");
	if (result != 0) {
		vm_https_cancel(request_id);
		vm_https_unset_channel(g_channel_id);
	} else {
		vm_log_debug("reply_content:%s", reply_segment);
		ret = vm_https_read_content(request_id, ++g_read_seg_num, 100);
		if (ret != 0) {
			vm_https_cancel(request_id);
			vm_https_unset_channel(g_channel_id);
		}
	}
}
static void https_send_read_read_content_rsp_cb(VMUINT16 request_id,
		VMUINT8 seq_num, VMUINT8 result, VMBOOL more, VMUINT8 *reply_segment,
		VMUINT32 reply_segment_len) {
	VMINT ret = -1;
	vm_log_debug("reply_content:%s", reply_segment);
	if (more > 0) {
		ret = vm_https_read_content(request_id, /* Request ID */
		++g_read_seg_num, /* Sequence number (for debug purpose) */
		100); /* The suggested segment data length of replied data in the peer buffer of
		 response. 0 means use reply_segment_len in MSG_ID_WPS_HTTP_REQ or
		 read_segment_length in previous request. */
		if (ret != 0) {
			vm_https_cancel(request_id);
			vm_https_unset_channel(g_channel_id);
		}
	} else {
		/* don't want to send more requests, so unset channel */
		vm_https_cancel(request_id);
		vm_https_unset_channel(g_channel_id);
		g_channel_id = 0;
		g_read_seg_num = 0;

	}
}
static void https_send_cancel_rsp_cb(VMUINT16 request_id, VMUINT8 result) {
	vm_log_debug("https_send_cancel_rsp_cb()");
}
static void https_send_status_query_rsp_cb(VMUINT8 status) {
	vm_log_debug("https_send_status_query_rsp_cb()");
}

void set_custom_apn(void) {
	vm_gsm_gprs_apn_info_t apn_info;

	memset(&apn_info, 0, sizeof(apn_info));
	strcpy(apn_info.apn, CUST_APN);
	strcpy(apn_info.proxy_address, PROXY_ADDRESS);
	apn_info.proxy_port = PROXY_PORT;
	apn_info.using_proxy = USING_PROXY;
	vm_gsm_gprs_set_customized_apn_info(&apn_info);
}

static void https_send_request(VM_TIMER_ID_NON_PRECISE timer_id,
		void* user_data) {
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	VMINT ret = -1;
	VMINT apn = VM_BEARER_DATA_ACCOUNT_TYPE_GPRS_CUSTOMIZED_APN;
	vm_https_callbacks_t callbacks = { https_send_request_set_channel_rsp_cb,
			https_unset_channel_rsp_cb, https_send_release_all_req_rsp_cb,
			https_send_termination_ind_cb, https_send_read_request_rsp_cb,
			https_send_read_read_content_rsp_cb, https_send_cancel_rsp_cb,
			https_send_status_query_rsp_cb };
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	do {
		set_custom_apn();
		vm_timer_delete_non_precise(timer_id);
		ret = vm_https_register_context_and_callback(apn, &callbacks);

		if (ret != 0) {
			break;
		}

		/* set network profile information */
		ret = vm_https_set_channel(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	} while (0);

}

void handle_sysevt(VMINT message, VMINT param) {
	switch (message) {
	case VM_EVENT_CREATE:
		vm_timer_create_non_precise(60000, https_send_request, NULL);
		break;

	case VM_EVENT_QUIT:
		break;
	}
}

void vm_main(void) {
	vm_pmng_register_system_event_callback(handle_sysevt);
}

