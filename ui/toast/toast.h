// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "ui/effects/animations.h"
#include "ui/text/text_entity.h"
#include "ui/click_handler.h"
#include "ui/rect_part.h"
#include "ui/rp_widget.h"
#include "base/weak_ptr.h"

namespace style {
struct Toast;
} // namespace style

namespace Ui {
namespace Toast {

namespace internal {
class Manager;
class Widget;
} // namespace internal

using ClickHandlerFilter = Fn<bool(const ClickHandlerPtr&, Qt::MouseButton)>;

inline constexpr auto kDefaultDuration = crl::time(1500);
struct Config {
	TextWithEntities text;
	not_null<const style::Toast*> st;
	crl::time durationMs = kDefaultDuration;
	int maxLines = 16;
	bool multiline = false;
	bool dark = false;
	RectPart slideSide = RectPart::None;
	ClickHandlerFilter filter;
	void *collapseKey = nullptr;
};

void SetDefaultParent(not_null<QWidget*> parent);

class Instance final : public base::has_weak_ptr {
	struct Private {
	};

public:
	Instance(
		const Config &config,
		not_null<QWidget*> widgetParent,
		bool animateAppearance,
		const Private &);
	Instance(const Instance &other) = delete;
	Instance &operator=(const Instance &other) = delete;

	void hideAnimated();
	void hide();

	[[nodiscard]] not_null<RpWidget*> widget() const;

private:
	void shownAnimationCallback();

	const not_null<const style::Toast*> _st;
	const crl::time _hideAt = 0;

	Ui::Animations::Simple _shownAnimation;
	bool _hiding = false;
	bool _sliding = false;
	void *_collapseKey = nullptr;

	// ToastManager should reset _widget pointer if _widget is destroyed.
	friend class internal::Manager;
	friend base::weak_ptr<Instance> Show(
		not_null<QWidget*> parent,
		const Config &config);
	std::unique_ptr<internal::Widget> _widget;

};

base::weak_ptr<Instance> Show(
	not_null<QWidget*> parent,
	const Config &config);
base::weak_ptr<Instance> Show(const Config &config);
base::weak_ptr<Instance> Show(
	not_null<QWidget*> parent,
	const QString &text);
base::weak_ptr<Instance> Show(const QString &text);

[[nodiscard]] base::weak_ptr<Instance> Last(void *key = nullptr);

} // namespace Toast
} // namespace Ui
