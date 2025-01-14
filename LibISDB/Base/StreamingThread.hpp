/*
  LibISDB
  Copyright(c) 2017-2020 DBCTRADO

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 @file   StreamingThread.hpp
 @brief  ストリーミングスレッド
 @author DBCTRADO
*/


#ifndef LIBISDB_STREAMING_THREAD_H
#define LIBISDB_STREAMING_THREAD_H


#include "../Utilities/Thread.hpp"
#include "../Utilities/ConditionVariable.hpp"
#include <atomic>


namespace LibISDB
{

	/** ストリーミングスレッドクラス */
	class StreamingThread
		: protected Thread
	{
	public:
		StreamingThread() noexcept;
		~StreamingThread();

		bool StartStreamingThread();
		void StopStreamingThread();

	protected:
	// Thread
		void ThreadMain() override;

		virtual void StreamingLoop();
		virtual bool ProcessStream() = 0;

		MutexLock m_StreamingThreadLock;
		ConditionVariable m_StreamingThreadCondition;
		std::atomic<bool> m_StreamingThreadEndSignal;
		std::chrono::milliseconds m_StreamingThreadTimeout;
		std::chrono::milliseconds m_StreamingThreadIdleWait;
	};

}	// namespace LibISDB


#endif	// ifndef LIBISDB_THREAD_H
