#include "QMemory.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

QMemory::QMemory ()
  : QMainWindow ()
{
  showFullScreen ();
}

QMemory::~QMemory ()
{}

void QMemory::keyPressEvent (QKeyEvent *event)
{
  qDebug () << event->key ();
  qApp->quit ();
}