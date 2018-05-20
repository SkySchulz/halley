#pragma once

#include "ui_scrollbar.h"
#include "ui_scroll_pane.h"

namespace Halley {
	class UIScrollBarPane : public UIWidget {
	public:
		UIScrollBarPane(Vector2f clipSize, UIStyle style, UISizer&& sizer, bool scrollHorizontal = false, bool scrollVertical = true, bool alwaysShow = true, Vector2f minSize = {});

		std::shared_ptr<UIScrollPane> getPane() const;

		void add(std::shared_ptr<UIWidget> widget, float proportion = 0, Vector4f border = Vector4f(), int fillFlags = UISizerFillFlags::Fill) override;
		void add(std::shared_ptr<UISizer> sizer, float proportion = 0, Vector4f border = Vector4f(), int fillFlags = UISizerFillFlags::Fill) override;
		void addSpacer(float size) override;
		void addStretchSpacer(float proportion = 0) override;

	private:
		std::shared_ptr<UIScrollPane> pane;
		std::shared_ptr<UIScrollBar> hBar;
		std::shared_ptr<UIScrollBar> vBar;
	};
}
