/*
 FS.cpp - file system wrapper
 Copyright (c) 2015 Ivan Grokhotkov. All rights reserved.
 This file is part of the esp8266 core for Arduino environment.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "FS.h"
#include "FSImpl.h"

using namespace fs;

static bool sflags(const char *mode, OpenMode& om, AccessMode& am);

// File

size_t File::write(const uint8_t *buf, size_t size) {
	if (!_file_impl) return 0;
	return _file_impl->write(buf, size);
}

size_t File::read(uint8_t* buf, size_t size) {
	if (!_file_impl) return -1;
	return _file_impl->read(buf, size);
}

int File::available() {
	if (!_file_impl) return false;
	return _file_impl->size() - _file_impl->position();
}

int File::read() {
	uint8_t result;
	if (read(&result, 1) != 1) {
		return -1;
	}
	return result;
}

int File::peek() {
	uint8_t result;
	if (read(&result, 1) != 1) {
		return -1;
	}
	seek(-1, SeekCur);
	return result;
}

void File::flush() {
	if (!_file_impl) return;
	_file_impl->flush();
}

bool File::seek(int32_t pos, SeekMode mode) {
	if (!_file_impl) return false;
	return _file_impl->seek(pos, mode);
}

size_t File::size() const {
	if (!_file_impl) return 0;
	return _file_impl->size();
}

time_t File::mtime() const {
	if (!_file_impl) {
		time_t ret{0};
		return ret;
	}
	return _file_impl->mtime();
}

size_t File::position() const {
	if (!_file_impl) return 0;
	return _file_impl->position();
}

const char *File::name() const {
	if (!_file_impl) return nullptr;
	return _file_impl->name();
}

File::operator bool() const {
	return !!_file_impl;
}

void File::close() {
	if (_file_impl) {
		_file_impl->close();
		_file_impl = nullptr;
	}
}

bool File::remove() {
	if (!_file_impl) return false;
	auto __file_impl = std::move(_file_impl);
	//_file_impl = nullptr;
	return __file_impl->remove();
}

bool File::rename(const char *pathTo) {
	if (!_file_impl) return false;
	auto __file_impl = std::move(_file_impl);
	//_file_impl = nullptr;
	return __file_impl->rename(pathTo);
}

// Dir

File Dir::openFile(const char *name, const char *mode) {
	if (!_dir_impl) return File();

	OpenMode om;
	AccessMode am;
	if (!sflags(mode, om, am)) {
		DEBUGV("Dir::openFile: invalid mode `%s`\n", mode);
		return File();
	}

	return File(_dir_impl->openFile(name, om, am));
}

Dir Dir::openDir(const char *name, bool create) {
	if (!_dir_impl) return Dir();
	return Dir(_dir_impl->openDir(name, create));
}

bool Dir::exists(const char *name) const {
	if (!_dir_impl) return false;
	return _dir_impl->exists(name);
}

bool Dir::isDir(const char *name) const {
	if (!_dir_impl) return false;
	return _dir_impl->isDir(name);
}

size_t Dir::size(const char *name) const {
	if (!_dir_impl) return 0;
	return _dir_impl->size(name);
}

time_t Dir::mtime(const char *name) const {
	if (!_dir_impl) return 0;
	return _dir_impl->mtime(name);
}

bool Dir::remove(const char *name) {
	if (!_dir_impl) return false;
	return _dir_impl->remove(name);
}

bool Dir::rename(const char *nameFrom, const char *nameTo) {
	if (!_dir_impl) return false;
	return _dir_impl->rename(nameFrom, nameTo);
}

bool Dir::next(bool reset) {
	if (!_dir_impl) return false;
	return _dir_impl->next(reset);
}

const char *Dir::entryName() const {
	if (!_dir_impl) return nullptr;
	return _dir_impl->entryName();
}

size_t Dir::entrySize() const {
	if (!_dir_impl) return 0;
	return _dir_impl->entrySize();
}

time_t Dir::entryMtime() const {
	if (!_dir_impl) return 0;
	return _dir_impl->entryMtime();
}

bool Dir::isEntryDir() const {
	if (!_dir_impl) return false;
	return _dir_impl->isEntryDir();
}

time_t Dir::mtime() const {
	if (!_dir_impl) return 0;
	return _dir_impl->mtime();
}

const char *Dir::name() const {
	if (!_dir_impl) return nullptr;
	return _dir_impl->name();
}

Dir::operator bool() const {
	return !!_dir_impl;
}

// FS

bool FS::begin() {
	if (!_fs_impl) return false;
	return _fs_impl->begin();
}

void FS::end() {
	if (_fs_impl) {
		_fs_impl->end();
	}
}

bool FS::format() {
	if (!_fs_impl) return false;
	return _fs_impl->format();
}

bool FS::info(FSInfo& info) const {
	if (!_fs_impl) return false;
	return _fs_impl->info(info);
}

File FS::openFile(const char *path, const char *mode) {
	if (!_fs_impl) return File();

	OpenMode om;
	AccessMode am;
	if (!sflags(mode, om, am)) {
		DEBUGV("FS::open: invalid mode `%s`\n", mode);
		return File();
	}

	return File(_fs_impl->openFile(path, om, am));
}

Dir FS::openDir(const char *path, bool create) {
	if (!_fs_impl) return Dir();
	return Dir(_fs_impl->openDir(path, create));
}

bool FS::exists(const char *path) const {
	if (!_fs_impl) return false;
	return _fs_impl->exists(path);
}

bool FS::isDir(const char *path) const {
	if (!_fs_impl) return false;
	return _fs_impl->isDir(path);
}

size_t FS::size(const char *path) const {
	if (!_fs_impl) return false;
	return _fs_impl->mtime(path);
}

time_t FS::mtime(const char *path) const {
	if (!_fs_impl) return false;
	return _fs_impl->mtime(path);
}

bool FS::remove(const char *path) {
	if (!_fs_impl) return false;
	return _fs_impl->remove(path);
}

bool FS::rename(const char *pathFrom, const char *pathTo) {
	if (!_fs_impl) return false;
	return _fs_impl->rename(pathFrom, pathTo);
}

// Utility functions

static bool sflags(const char *mode, OpenMode& om, AccessMode& am) {
	switch (mode[0]) {
		case 'r':
			am = AM_READ;
			om = OM_DEFAULT;
			break;
		case 'w':
			am = AM_WRITE;
			om = (OpenMode) (OM_CREATE | OM_TRUNCATE);
			break;
		case 'a':
			am = AM_WRITE;
			om = (OpenMode) (OM_CREATE | OM_APPEND);
			break;
		default:
			return false;
	}
	switch(mode[1]) {
		case '+':
			am = (AccessMode) (AM_WRITE | AM_READ);
			break;
		case 0:
			break;
		default:
			return false;
	}
	return true;
}


#if defined(FS_FREESTANDING_FUNCTIONS)

/*
TODO: move these functions to public API:
*/
// Arduino legacy API compatibility
File open(const char *path, const char *mode);

File openFile(const char *path, const char *mode);
File openFile(String& path, const char *mode);

Dir openDir(const char *path, bool create = false);
Dir openDir(String& path, bool create = false);

template<>
bool mount<FS>(FS& fs, const char *mountPoint);

struct MountEntry {
	FSImplPtr fs;
	String path;
	MountEntry* next;
};

static MountEntry* s_mounted = nullptr;

template<>
bool mount<FS>(FS& fs, const char *mp) {
	FSImplPtr p = fs._fs_impl;
	if (!p || !p->mount()) {
		DEBUGV("FSImpl mount failed\n");
		return false;
	}

	if (!mp || *mp != '/') {
		DEBUGV("Invalid mount point\n");
		return false;
	}

	MountEntry* entry = new MountEntry;
	entry->fs = p;
	entry->path = mp;
	entry->next = s_mounted;
	s_mounted = entry;
	return true;
}


/*
  iterate over MountEntries and look for the ones which match the path
*/
File openFile(String const &path, const char *mode) {
	for (MountEntry* entry = s_mounted; entry; entry = entry->next) {
		size_t offset = entry->path.length();
		if (path.startsWith(entry->path)) {
			File result = entry->fs->openFile(path.begin()+offset, mode);
			if (result) return result;
		}
	}
	return File();
}

Dir openDir(String const &path, bool create) {
	for (MountEntry* entry = s_mounted; entry; entry = entry->next) {
		size_t offset = entry->path.length();
		if (path.startsWith(entry->path)) {
			Dir result = entry->fs->openDir(path.begin()+offset, create);
			if (result) return result;
		}
	}
	return Dir();
}

File openFile(const char *path, const char *mode) {
	return FS::openFile(path, mode);
}

File open(const char *path, const char *mode) {
	return FS::openFile(path, mode);
}

Dir openDir(const char *path, bool create) {
	return FS::openDir(path.c_str(), create);
}
#endif
