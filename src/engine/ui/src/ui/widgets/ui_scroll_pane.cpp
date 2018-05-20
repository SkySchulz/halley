#include "widgets/ui_scroll_pane.h"
#include "ui_style.h"

using namespace Halley;

UIScrollPane::UIScrollPane(Vector2f clipSize, UISizer&& sizer, bool scrollHorizontal, bool scrollVertical)
	: UIWidget("", Vector2f(), std::move(sizer))
	, clipSize(clipSize)
	, scrollSpeed(10.0f)
	, scrollHorizontal(scrollHorizontal)
	, scrollVertical(scrollVertical)
{
	setHandle(UIEventType::MouseWheel, [this] (const UIEvent& event)
	{
		onMouseWheel(event);
	});

	setHandle(UIEventType::MakeAreaVisible, [this] (const UIEvent& event)
	{
		scrollToShow(event.getRectData(), false);
	});

	setHandle(UIEventType::MakeAreaVisibleCentered, [this] (const UIEvent& event)
	{
		scrollToShow(event.getRectData(), true);
	});
}

Vector2f UIScrollPane::getScrollPosition() const
{
	return scrollPos;
}

Vector2f UIScrollPane::getRelativeScrollPosition() const
{
	return scrollPos / contentsSize;
}

void UIScrollPane::scrollTo(Vector2f position)
{	
	if (scrollHorizontal) {
		scrollPos.x = clamp2(position.x, 0.0f, contentsSize.x - getSize().x);
	}
	
	if (scrollVertical) {
		scrollPos.y = clamp2(position.y, 0.0f, contentsSize.y - getSize().y);
	}
}

void UIScrollPane::scrollBy(Vector2f delta)
{
	scrollTo(scrollPos + delta);
}

void UIScrollPane::setScrollSpeed(float speed)
{
	scrollSpeed = speed;
}

void UIScrollPane::update(Time t, bool moved)
{
	if (!scrollHorizontal) {
		clipSize.x = getSize().x;
		scrollPos.x = 0;
	}
	if (!scrollVertical) {
		clipSize.y = getSize().y;
		scrollPos.y = 0;
	}
	contentsSize = UIWidget::getLayoutMinimumSize(false);
	setMouseClip(Rect4f(getPosition(), getPosition() + getSize()));
}

bool UIScrollPane::canScroll(UIScrollDirection direction) const
{
	auto contentsSize = UIWidget::getLayoutMinimumSize(false);
	if (direction == UIScrollDirection::Horizontal) {
		return scrollHorizontal && getSize().x < contentsSize.x;
	} else {
		return scrollVertical && getSize().y < contentsSize.y;
	}
}

float UIScrollPane::getCoverageSize(UIScrollDirection direction) const
{
	if (direction == UIScrollDirection::Horizontal) {
		return getSize().x / contentsSize.x;
	} else {
		return getSize().y / contentsSize.y;
	}
}

void UIScrollPane::drawChildren(UIPainter& painter) const
{
	auto p = painter.withClip(Rect4f(getPosition(), getPosition() + getSize()));
	UIWidget::drawChildren(p);
}

Vector2f UIScrollPane::getLayoutMinimumSize(bool force) const
{
	auto size = UIWidget::getLayoutMinimumSize(false);
	if (scrollHorizontal) {
		size.x = std::min(size.x, clipSize.x);
	}
	if (scrollVertical) {
		size.y = std::min(size.y, clipSize.y);
	}
	return size;
}

bool UIScrollPane::canInteractWithMouse() const
{
	return true;
}

void UIScrollPane::onMouseWheel(const UIEvent& event)
{
	scrollBy(Vector2f(0.0f, -scrollSpeed * event.getIntData()));
}

Vector2f UIScrollPane::getLayoutOriginPosition() const
{
	return getPosition() - scrollPos.floor();
}

void UIScrollPane::scrollToShow(Rect4f rect, bool center)
{
	auto size = getSize();

	float maxX = rect.getLeft();
	float minX = rect.getRight() - size.x;
	float maxY = rect.getTop();
	float minY = rect.getBottom() - size.y;
	auto target = center ? (rect.getCenter() - 0.5f * size) : scrollPos;
	scrollTo(Vector2f(clamp(target.x, minX, maxX), clamp(target.y, minY, maxY)));
}

float UIScrollPane::getScrollSpeed() const
{
	return scrollSpeed;
}

void UIScrollPane::setRelativeScroll(float position, UIScrollDirection direction)
{
	int axis = direction == UIScrollDirection::Horizontal ? 0 : 1;
	auto target = scrollPos;
	target[axis] = position * contentsSize[axis];
	scrollTo(target);
}
