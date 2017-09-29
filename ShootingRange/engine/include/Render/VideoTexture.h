#pragma once

#include "ogg/ogg.h"
#include "theora/theora.h"
#include "Animation.h"

namespace Render {
	class VideoTexture : public Animation
	{
	private:
		FILE *_file;
		BYTE *_frame;
		ogg_stream_state OGG_STREAM_STATE_THEORA;
		ogg_sync_state OGG_SYNC_STATE;
		ogg_page OGG_PAGE;
		ogg_packet OGG_PACKET;
		theora_comment THEORA_COMMENT;
		theora_info THEORA_INFO;
		theora_state THEORA_STATE;
		yuv_buffer YUV_BUFFER;

	public:
		VideoTexture();
		~VideoTexture();

		bool							Upload(const string &filename);
		void							Release();
		virtual float					Update(float dt);
		virtual void					setCurrentFrame(int iFrame);
	};
}
