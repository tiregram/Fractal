
#include <iostream>
#include <string>

#include "Image.hpp"
#include "Parser.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <QWidget>
#include <QtQuick>


#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

//#include <QtWidgets/QPushButton>
// string readAllFile(ifstream &in) {
//   stringstream sstr;
//   sstr << in.rdbuf();
//   return sstr.str();
// }

// class ColorImageProvider : public QQuickImageProvider {
// public:
//   ColorImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

//   QPixmap requestPixmap(const QString &id, QSize *size,
//                         const QSize &requestedSize) {
//     int width = 100;
//     int height = 50;

//     if (size)
//       *size = QSize(width, height);
//     QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
//                    requestedSize.height() > 0 ? requestedSize.height()
//                                               : height);
//     pixmap.fill(QColor(id).rgba());

//     return pixmap;
//   }
// };

class FractImageProvider : public QQuickImageProvider {
private:
  OpenCLWrapper *oc;

public:
  FractImageProvider(OpenCLWrapper *oc)
      : QQuickImageProvider(QQmlImageProviderBase::Image, 0) {
    this->oc = oc;
  }

  QImage requestImage(const QString &id, QSize *size,
                      const QSize &requestedSize) {

    const int width = requestedSize.width();
    const int height = requestedSize.height();

    std::cout << id.toUtf8().constData() << "\n";
    size->setWidth(width);
    size->setHeight(height);
    std::string uristr = id .toUtf8().constData();

    std::vector<std::string> arg;
    boost::split(arg, uristr, boost::is_any_of("/"));

    uchar * data= (uchar*)oc->run(height, width, 0, 0.68, atof(arg[3].c_str()) , atof(arg[4].c_str()), atof(arg[2].c_str()),(arg[5][0]=='t')?1:0);
    uchar *ite = data;

    for (unsigned int x = 0; x < width*height; ++x) {
      ite++; // r
      ite++; // G
      ite++; // b
      *(ite++) = 255;
    }


    QImage image(data, width, height, QImage::Format_ARGB32);
    delete data;
    return image;
  }
};

int main_graphique(int argc, char *argv[], OpenCLWrapper *oc) {
  QGuiApplication app(argc, argv);

  QQuickView viewer;
  QQmlEngine *engine = viewer.engine();

  auto a = new FractImageProvider(oc);

  //  engine->addImageProvider(QLatin1String("colors"), new
  //  ColorImageProvider());
  engine->addImageProvider("fract", a);

  viewer.setSource(QUrl::fromLocalFile("src/qml/test.qml"));
  viewer.show();

  return app.exec();
}

int main(int argc, char *argv[]) {

  auto oc = OpenCLWrapper();

  oc.print_plat_dev();

  std::cout << "select platforms.device platforms.device default(0.0):"
            << "\n";

  std::string plat_dev;
  std::getline(std::cin, plat_dev);
  std::cout << plat_dev << "\n";

  oc.selectByString(plat_dev);
  
  oc.print_plat_dev();

  oc.loadSource("ocl_src/frac.c", 1920, 1080);

  main_graphique(argc, argv, &oc);

}

#include "main.moc"
