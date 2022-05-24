#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#if 0
	device -> device handle (open and close)-> detach kernel driver 
		-> interface (claim and release)
#endif

#define evofox_vendor_id 0x18f8
#define evofox_product_id 0x1286
#define evofox_bmRequestType 0x21
#define evofox_bRequest 0x09
#define evofox_wValue 0x0307
#define evofox_wIndex 0x01
#define evofox_wLength 0x08
#define evofox_interfaceID 0x01
#define evofox_endpointID 0
#define evofox_data_length 8

unsigned char evofox_led_first13[14 * 8] = {
	0x07, 0x01, 0x01, 0x01, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x01, 0x03, 0x03, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x01, 0x02, 0x02, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x01, 0x05, 0x05, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x01, 0x04, 0x04, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x01, 0x08, 0x0b, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x01, 0x06, 0x0c, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x12, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x07, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x07, 0x09, 0x02, 0x48, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x09, 0x02, 0x69, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x09, 0x02, 0x8a, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x09, 0x02, 0xcb, 0x0f, 0x00, 0x00, 0x00,
	0x07, 0x13, 0x7f, 0x13, 0x0f, 0x00, 0x00, 0x00
};

unsigned char evofox_led_slow[evofox_data_length]   = 	{0x07, 0x13, 0x7f, 0x10, 0x0f, 0x00, 0x00, 0x00};
unsigned char evofox_led_fast[evofox_data_length] 	= 	{0x07, 0x13, 0x7f, 0x13, 0x0f, 0x00, 0x00, 0x00};
unsigned char evofox_led_static[evofox_data_length] = 	{0x07, 0x13, 0x7f, 0x16, 0x0f, 0x00, 0x00, 0x00};
unsigned char evofox_led_off[evofox_data_length] 	= 	{0x07, 0x13, 0x7f, 0x17, 0x0f, 0x00, 0x00, 0x00};

libusb_device **list;

libusb_device_handle *open_device_with_vid_pid(uint16_t vid, uint16_t pid) {
	int cnt = libusb_get_device_list(NULL, &list);
	if (cnt < 0) {
		fprintf(stderr, "No devices available.\n");
		return NULL;
	}

	libusb_device *tmp;
	struct libusb_device_descriptor dev;
	libusb_device_handle *hndl;
	
	int rc;
	for (int i = 0; i < cnt; ++i) {
		tmp = list[i];
		if ((rc = libusb_get_device_descriptor(tmp, &dev)) != 0) {
			fprintf(stderr, "%s\n", libusb_strerror(rc));
			return NULL;
		}
		if (dev.idVendor == evofox_vendor_id && dev.idProduct == evofox_product_id) {
			if ((rc = libusb_open(tmp, &hndl)) != 0) {
				fprintf(stderr, "%s\n", libusb_strerror(rc));
				return NULL;
			}
			return hndl;
		}
	}
	return NULL;
}



int main(int argc, char *argv[]) {
	if (libusb_init(NULL) != 0) {
		fprintf(stderr, "Could not Initialize libusb\n");
	}
	libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
	libusb_device_handle *efx_handle = open_device_with_vid_pid(
			evofox_vendor_id, evofox_product_id);
	if (efx_handle == NULL) {
		exit(1);
	}

#if 0
	if (libusb_set_auto_detach_kernel_driver(efx_handle, 1) != LIBUSB_SUCCESS) {
		fprintf(stderr, "Does not support automatic kernel detach driver\n");
		exit(1);
	}
#endif

	int rc = libusb_kernel_driver_active(efx_handle, evofox_interfaceID);
	if (rc == 1) {
		if ((rc = libusb_detach_kernel_driver(efx_handle, evofox_interfaceID)) != 0) {
			fprintf(stderr, "%s\n", libusb_strerror(rc));
			exit(1);
		}
	}
	if ((rc = libusb_claim_interface(efx_handle, evofox_interfaceID)) != 0) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		exit(1);
	}
	
 	/* memcpy(&evofox_led_first13[13*8], evofox_led_off, evofox_data_length); */
	if ((rc = libusb_control_transfer(efx_handle, evofox_bmRequestType, evofox_bRequest,
			evofox_wValue, evofox_wIndex, evofox_led_first13, sizeof(evofox_led_first13), 5000)) != 0) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		exit(1);
	}

	libusb_release_interface(efx_handle, evofox_interfaceID);
	libusb_attach_kernel_driver(efx_handle, evofox_interfaceID);
	libusb_close(efx_handle);
	libusb_free_device_list(list, 1);
	libusb_exit(NULL);
}
