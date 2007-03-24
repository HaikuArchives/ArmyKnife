#ifndef _AUDIO_INFO_H__
#define _AUDIO_INFO_H__

class AudioInfo
{
	public:
		AudioInfo();
		virtual ~AudioInfo();

		virtual const char* Artist();
		virtual const char* Album();
		virtual const char* Title();
		virtual const char* Year();
		virtual const char* Comment();
		virtual const char* Track();
		virtual const char* Genre();

		virtual void SetArtist(const char* value);
		virtual void SetAlbum(const char* value);
		virtual void SetTitle(const char* value);
		virtual void SetYear(const char* value);
		virtual void SetComment(const char* value);
		virtual void SetTrack(const char* value);
		virtual void SetGenre(const char* value);

		virtual status_t Read();
		virtual status_t ReadArtist();
		virtual status_t ReadAlbum();
		virtual status_t ReadTitle();
		virtual status_t ReadYear();
		virtual status_t ReadComment();
		virtual status_t ReadTrack();
		virtual status_t ReadGenre();

		virtual status_t Write();
		virtual status_t WriteArtist();
		virtual status_t WriteAlbum();
		virtual status_t WriteTitle();
		virtual status_t WriteYear();
		virtual status_t WriteComment();
		virtual status_t WriteTrack();
		virtual status_t WriteGenre();

		virtual void PrintToStream();
};

#endif

