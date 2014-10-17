#include "screenswidget.h"

void intToRGB(int col, double &r, double &g, double &b)
{
  r = (double)(col >> 16)/255;
  g = (double)((col & 0x0000FF00) >> 8)/255;
  b = (double)(col & 0x000000FF)/255;
}

ScreensWidget::ScreensWidget()
{
  font.set_family("Unifont");
  font.set_weight(Pango::WEIGHT_NORMAL);
  font.set_size(1024 * 8);
  fontLayout = create_pango_layout("");
  fontLayout->set_font_description(font);
}

ScreensWidget::~ScreensWidget()
{

}

void ScreensWidget::addScreen(std::shared_ptr< ComponentScreen > screen)
{
  screens.push_back(screen);
}

bool ScreensWidget::on_draw(const Cairo::RefPtr< Cairo::Context >& cr)
{
  
  /*cr->set_source_rgb(1, 0, 0);
  
  cr->rectangle(0, 0, 40, 70);
  cr->fill();
  
  fontLayout->set_text("a");
  cr->move_to(40, 40);
  fontLayout->show_in_cairo_context(cr);*/
  
  int pixX = 8;
  int pixY = 16;
  int offsetX = 10;
  int offsetY = 10;
  
  double r, g, b;
  
  for (screenIterator it = screens.begin(); it != screens.end(); ++it)
  {
    Glib::Threads::Mutex::Lock lock((*it)->mut);
    // std::cout << "Redrawing" << std::endl;
    for (int x = 0; x < (*it)->getWidth(); ++x)
    {
      for (int y = 0; y < (*it)->getHeight(); ++y)
      {
	intToRGB((*it)->pixels[x][y].bColor, r, g, b);
	cr->set_source_rgb(r, g, b);
	cr->rectangle(offsetX + (pixX * x), offsetY + (pixY * y), pixX, pixY);
	cr->fill();
	
	intToRGB((*it)->pixels[x][y].tColor, r, g, b);
	cr->set_source_rgb(r, g, b);
	fontLayout->set_text(Glib::ustring(1, (*it)->pixels[x][y].text).c_str());
	cr->move_to(offsetX + (pixX * x), offsetY + (pixY * y));
	fontLayout->show_in_cairo_context(cr);
      }
    }
  }
  
  return true;
}