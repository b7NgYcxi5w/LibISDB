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
 @file   VideoRenderer.hpp
 @brief  映像レンダラ
 @author DBCTRADO
*/


#ifndef LIBISDB_VIDEO_RENDERER_H
#define LIBISDB_VIDEO_RENDERER_H


#include "../../../../Base/ErrorHandler.hpp"
#include "../../../Utilities/COMUtilities.hpp"
#include <dshow.h>


namespace LibISDB::DirectShow
{

	/** 映像レンダラ基底クラス */
	class VideoRenderer
		: public ErrorHandler
	{
	public:
		/** 映像レンダラの種類 */
		enum class RendererType {
			Invalid = -1,       /**< 無効 */
			Default,            /**< デフォルトレンダラ */
			VMR7,               /**< VMR-7 */
			VMR9,               /**< VMR-9 */
			VMR7Renderless,     /**< VMR-7 Renderless */
			VMR9Renderless,     /**< VMR-9 Renderless */
			EVR,                /**< EVR */
			OverlayMixer,       /**< Overlay Mixer */
			madVR,              /**< madVR */
			EVRCustomPresenter, /**< EVR Custom Presenter */
			MPCVideoRenderer,   /**< MPC Video Renderer */
		};

		VideoRenderer() = default;
		virtual ~VideoRenderer();

		virtual RendererType GetRendererType() const noexcept = 0;
		virtual bool Initialize(
			IGraphBuilder *pGraphBuilder, IPin *pInputPin,
			HWND hwndRender, HWND hwndMessageDrain) = 0;
		virtual bool Finalize();
		virtual bool SetVideoPosition(
			int SourceWidth, int SourceHeight, const RECT &SourceRect,
			const RECT &DestRect, const RECT &WindowRect) = 0;
		virtual bool GetDestPosition(ReturnArg<RECT> Rect) = 0;
		virtual COMMemoryPointer<> GetCurrentImage() { return COMMemoryPointer<>(); }
		virtual bool ShowCursor(bool Show) { return true; }
		virtual bool RepaintVideo(HWND hwnd, HDC hdc) { return true; }
		virtual bool DisplayModeChanged() { return true; }
		virtual bool SetVisible(bool Visible) { return true; }
		virtual bool ShowProperty(HWND hwndOwner);
		virtual bool HasProperty();
		IBaseFilter * GetRendererFilter() const { return m_Renderer.Get(); }
		HWND GetVideoWindow() const noexcept { return m_hwndVideo; }
		virtual bool SetCrop1088To1080(bool Crop) { return false; }
		virtual bool SetClipToDevice(bool Clip) { return false; }

		static VideoRenderer * CreateRenderer(RendererType Type);
		static LPCTSTR EnumRendererName(int Index);
		static LPCTSTR EnumRendererName(RendererType Type) { return EnumRendererName(static_cast<int>(Type)); }
		static RendererType ParseName(LPCTSTR pszName);
		static bool IsAvailable(RendererType Type);

	protected:
		COMPointer<IBaseFilter> m_Renderer;
		COMPointer<IGraphBuilder> m_GraphBuilder;
		HWND m_hwndRender = nullptr;
		HWND m_hwndVideo = nullptr;
		bool m_Crop1088To1080 = true;
		bool m_ClipToDevice = true;
	};

	/** デフォルト映像レンダラクラス */
	class VideoRenderer_Default
		: public VideoRenderer
	{
	public:
		RendererType GetRendererType() const noexcept override { return RendererType::Default; }
		bool Initialize(
			IGraphBuilder *pGraphBuilder, IPin *pInputPin,
			HWND hwndRender, HWND hwndMessageDrain) override;
		bool Finalize() override;
		bool SetVideoPosition(
			int SourceWidth, int SourceHeight, const RECT &SourceRect,
			const RECT &DestRect, const RECT &WindowRect) override;
		bool GetDestPosition(ReturnArg<RECT> Rect) override;
		COMMemoryPointer<> GetCurrentImage() override;
		bool ShowCursor(bool Show) override;
		bool SetVisible(bool Visible) override;

	protected:
		bool InitializeBasicVideo(IGraphBuilder *pGraphBuilder, HWND hwndRender, HWND hwndMessageDrain);
		virtual HWND FindVideoWindow();

		COMPointer<IVideoWindow> m_VideoWindow;
		COMPointer<IBasicVideo> m_BasicVideo;
	};

	/** 映像レンダラ標準実装クラス */
	class VideoRenderer_Basic
		: public VideoRenderer_Default
	{
	public:
		VideoRenderer_Basic(const CLSID &clsid, LPCTSTR pszName, bool NoSourcePosition = false) noexcept;

		bool Initialize(
			IGraphBuilder *pGraphBuilder, IPin *pInputPin,
			HWND hwndRender, HWND hwndMessageDrain) override;
		bool SetVideoPosition(
			int SourceWidth, int SourceHeight, const RECT &SourceRect,
			const RECT &DestRect, const RECT &WindowRect) override;

	protected:
		CLSID m_clsidRenderer;
		String m_RendererName;
		bool m_NoSourcePosition;
	};

}	// namespace LibISDB::DirectShow


#endif	// ifndef LIBISDB_VIDEO_RENDERER_H
