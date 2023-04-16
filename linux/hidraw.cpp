#include "hidraw.h"
#include "exceptions.h"

// posix includes
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

// cpp includes
#include <cstring>

void hidraw::hid_init() {
	device_handle = open(hid_dev_path.c_str(), O_RDWR);
	if(device_handle < 0) throw(hid_error("Could not init device. " + get_error(errno)));
}

void hidraw::hid_deinit() {
	if (device_handle != -1) {
		close(device_handle);
		device_handle = -1;
	}
}

hidraw::hidraw(std::string dev_path) : hid_dev_path{dev_path} {
	hid_init();
}

void hidraw::hid_write(const std::string &data) {
	unsigned long data_len{ data.length() };
	if (data_len < 1) {
		return;
	} else if (data_len < 2) {
		data_len = 2;
	}

	long result{ write(device_handle, data.c_str(), data_len) };
	if(result < 0) throw(hid_error("write error." + get_error(errno)));
}

void hidraw::hid_read(std::string &data) {
	char buffer[256];
	long result{ read(device_handle, buffer, 256) };
	if (result < 0) {
		data = "";
		throw(hid_error("read error. " + get_error(errno)));
	}
	buffer[result] = '\0';
	data = std::string(buffer);
}

hidraw::~hidraw() {
	hid_deinit();
}

std::string hidraw::get_error(const int &err) {
	return std::string(std::strerror(err));
}
