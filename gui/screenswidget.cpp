#include "screenswidget.h"

ScreensWidget::ScreensWidget() {
}

ScreensWidget::~ScreensWidget() {

}

void ScreensWidget::addScreen(std::shared_ptr<ComponentScreen> screen) {
    screens.push_back(screen);
}

bool ScreensWidget::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->set_source_rgb(0, 0.2, 0.7);
    cr->rectangle(0, 0, get_width(), get_height());
    cr->fill();
    for (screenIterator it = screens.begin(); it != screens.end(); ++it) {
        Glib::Threads::Mutex::Lock lock((*it)->mut);
        cr->set_source((*it)->img, 10, 10);
        cr->paint();
    }


    return true;
}