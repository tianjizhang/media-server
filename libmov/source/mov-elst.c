#include "file-reader.h"
#include "mov-internal.h"
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

// 8.6.6 Edit List Box (p53)
int mov_read_elst(struct mov_t* mov, const struct mov_box_t* box)
{
	uint32_t i, entry_count;
	uint32_t version, flags;
	struct mov_track_t* track = mov->track;

	version = file_reader_r8(mov->fp); /* version */
	flags = file_reader_rb24(mov->fp); /* flags */
	entry_count = file_reader_rb32(mov->fp);

	assert(0 == track->elst_count && NULL == track->elst);
	if (track->elst_count < entry_count)
	{
		void* p = realloc(track->elst, sizeof(struct mov_elst_t) * entry_count);
		if (NULL == p) return ENOMEM;
		track->elst = (struct mov_elst_t*)p;
	}
	track->elst_count = entry_count;

	for (i = 0; i < entry_count; i++)
	{
		if (1 == version)
		{
			track->elst[i].segment_duration = file_reader_rb64(mov->fp);
			track->elst[i].media_time = (int64_t)file_reader_rb64(mov->fp);
		}
		else
		{
			assert(0 == version);
			track->elst[i].segment_duration = file_reader_rb32(mov->fp);
			track->elst[i].media_time = (int32_t)file_reader_rb32(mov->fp);
		}
		track->elst[i].media_rate_integer = (int16_t)file_reader_rb16(mov->fp);
		track->elst[i].media_rate_fraction = (int16_t)file_reader_rb16(mov->fp);
	}

	return file_reader_error(mov->fp);
}

size_t mov_write_elst(const struct mov_t* mov)
{
	return 0;
}
