#ifndef INCLUDE_COMM_SYSTEM_FILESYSTEM_MESSAGE_H
#define INCLUDE_COMM_SYSTEM_FILESYSTEM_MESSAGE_H

#include <vector>
namespace comm {
	/**
	*	FileSystem Actions
	*/
	enum class ActionType : std::size_t {
		None = 0,
		Open,
		Save
	};

	/**
	*	File type categories
	*/
	enum class FileType : std::size_t {
		None = 0,
		Project,
		Graphics,
		Video,
		Image,
		PerfCap,
		PTX
	};

	/**
	*	Filesystem Message
	*/
	class FilesystemMessage {
	public:
		FilesystemMessage(const std::string& filepath, const ActionType& actionType, const FileType& filetype);
		const ActionType action() const { return mAction; }
		const std::string filepath() const { return mFilepath; }
		const FileType fileType() const { return mFileType; }
	private:
		ActionType mAction;
		FileType mFileType;
		std::string mFilepath;
	};

	/**
	*	Constructs Filesystem Message
	*/
	FilesystemMessage::FilesystemMessage(const std::string& filepath, const ActionType& actionType, const FileType& filetype) {
		mAction = actionType;
		mFileType = filetype;
		mFilepath = filepath;
	}
}	//	!namespace comm
#endif // !INCLUDE_COMM_SYSTEM_FILESYSTEM_MESSAGE_H

