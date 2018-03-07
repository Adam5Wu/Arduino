/*
 FS.h - file system wrapper
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

#ifndef FS_H
#define FS_H

#include <memory>
#include <Arduino.h>

namespace fs {

class File;
class Dir;

class FileImpl;
typedef std::shared_ptr<FileImpl> FileImplPtr;
class FSImpl;
typedef std::shared_ptr<FSImpl> FSImplPtr;
class DirImpl;
typedef std::shared_ptr<DirImpl> DirImplPtr;

template <typename Tfs>
bool mount(Tfs& fs, const char *mountPoint);

enum SeekMode {
	SeekSet = 0,
	SeekCur = 1,
	SeekEnd = 2
};

class FS;

class File : public Stream {
public:
	File(FileImplPtr file_impl = FileImplPtr()) : _file_impl(file_impl) {}

	// Print methods:
	size_t write(const uint8_t *buf, size_t size) override;
	size_t write(uint8_t c) override
	{ return write(&c, 1); }

	size_t read(uint8_t* buf, size_t size);
	size_t readBytes(char *buffer, size_t length) override
	{ return read((uint8_t*)buffer, length); }

	// Stream methods:
	int available() override;
	int read() override;
	int peek() override;
	void flush() override;

	bool seek(int32_t pos, SeekMode mode);
	bool seek(int32_t pos)
	{ return seek(pos, SeekSet); }

	size_t size() const;
	time_t mtime() const;
	size_t position() const;

	const char *name() const;
	operator bool() const;

	void close();
	bool remove();
	bool rename(const char *nameTo);
	bool rename(String const &nameTo)
	{ return rename(nameTo.c_str()); }

protected:
	FileImplPtr _file_impl;
};

class Dir {
public:
	Dir(DirImplPtr dir_impl = DirImplPtr()): _dir_impl(dir_impl) { }

	// Arduino legacy API compatibility
	File openFile(const char *mode)
	{ return openEntryFile(mode); }
	String fileName()
	{ return entryName(); }
	size_t fileSize()
	{ return entrySize(); }

	File openFile(const char *name, const char *mode);
	File openFile(String const &name, const char *mode)
	{ return openFile(name.c_str(), mode); }

	Dir openDir(const char *name, bool create = false);
	Dir openDir(String const &name, bool create = false)
	{ return openDir(name.c_str(), create); }

	bool exists(const char *name) const;
	bool exists(String const &name) const
	{ return exists(name.c_str()); }

	bool isDir(const char *name) const;
	bool isDir(String const &name) const
	{ return isDir(name.c_str()); }

	size_t size(const char *name) const;
	size_t size(String const &name) const
	{ return size(name.c_str()); }

	time_t mtime(const char *name) const;
	time_t mtime(String const &name) const
	{ return mtime(name.c_str()); }

	bool remove(const char *name);
	bool remove(String const &name)
	{ return remove(name.c_str()); }

	bool rename(const char *nameFrom, const char *nameTo);
	bool rename(String const &nameFrom, String const &nameTo)
	{ return rename(nameFrom.c_str(), nameTo.c_str()); }

	bool next(bool reset = false);

	const char *entryName() const;
	size_t entrySize() const;
	time_t entryMtime() const;
	bool isEntryDir() const;

	File openEntryFile(const char *mode)
	{ return openFile(entryName(), mode); }
	Dir openEntryDir()
	{ return openDir(entryName()); }
	bool removeEntry()
	{ return remove(entryName()); }
	bool renameEntry(const char *nameTo)
	{ return rename(entryName(), nameTo); }
	bool renameEntry(String const &nameTo)
	{ return renameEntry(nameTo.c_str()); }

	time_t mtime() const;
	const char *name() const;
	operator bool() const;

protected:
	DirImplPtr _dir_impl;
};

struct FSInfo {
	size_t totalBytes;
	size_t usedBytes;
	size_t blockSize;
	size_t pageSize;
	size_t maxOpenFiles;
	size_t maxPathLength;
};

class FS {
public:
	FS(FSImplPtr fs_impl) : _fs_impl(fs_impl) { }

	bool begin();
	void end();

	bool format();
	bool info(FSInfo& info) const;

	// Arduino legacy API compatibility
	File open(const char *path, const char *mode)
	{ return openFile(path, mode); }
	File open(String const &path, const char *mode)
	{ return openFile(path.c_str(), mode); }

	File openFile(const char *path, const char *mode);
	File openFile(String const &path, const char *mode)
	{ return openFile(path.c_str(), mode); }

	Dir openDir(const char *path, bool create = false);
	Dir openDir(String const &path, bool create = false)
	{ return openDir(path.c_str(), create); }

	bool exists(const char *path) const;
	bool exists(String const &path) const
	{ return exists(path.c_str()); }

	bool isDir(const char *path) const;
	bool isDir(String const &path) const
	{ return isDir(path.c_str()); }

	size_t size(const char *path) const;
	size_t size(String const &path) const
	{ return size(path.c_str()); }

	time_t mtime(const char *path) const;
	time_t mtime(String const &path) const
	{ return mtime(path.c_str()); }

	bool remove(const char *path);
	bool remove(String const &path)
	{ return remove(path.c_str()); }

	bool rename(const char *pathFrom, const char *pathTo);
	bool rename(String const &pathFrom, String const &pathTo)
	{ return rename(pathFrom.c_str(), pathTo.c_str()); }

protected:
	FSImplPtr _fs_impl;
};

} // namespace fs

#ifndef FS_NO_GLOBALS
using fs::FS;
using fs::File;
using fs::Dir;
using fs::SeekMode;
using fs::SeekSet;
using fs::SeekCur;
using fs::SeekEnd;
using fs::FSInfo;
#endif //FS_NO_GLOBALS

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SPIFFS)
extern fs::FS SPIFFS;
#endif

#endif //FS_H
