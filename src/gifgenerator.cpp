/*  exmplayer, GUI front-end for mplayer.
    Copyright (C) 2010-2015 Rupesh Sreeraman

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
#include "gifgenerator.h"

GifGenerator::GifGenerator(QObject *parent) :
    QObject(parent)
{
    ffmpegProcess=new MyProcess(this);
    QObject::connect(ffmpegProcess,SIGNAL(lineAvailable(QByteArray)),this,SLOT(ffmpegConsole(QByteArray)));
    QObject::connect(ffmpegProcess,SIGNAL(finished(int)),this,SLOT(emitProcessFinished(int)));

}
void GifGenerator::setFfmpegOptions(QString filename,double startPos,short duration){

    QString ffmpegBinPath;
    //ffmpeg -v warning -ss 6767 -t 2  -i %1 -i %palette% -lavfi "%filters% [x]; [x][1:v] paletteuse" -y %2
#ifdef Q_OS_WIN
    ffmpegBinPath=qApp->applicationDirPath()+"/ffmpeg.exe";
#endif
#ifdef Q_OS_LINUX
    ffmpegBinPath=Paths::sharePath()+"/ffmpeg";

#endif
    ffmpegProcess->clearArguments();
    ffmpegProcess->addArgument(ffmpegBinPath);
    ffmpegProcess->addArgument("-ss");
    ffmpegProcess->addArgument(QString::number(6767));
    ffmpegProcess->addArgument("-t");
    ffmpegProcess->addArgument(QString::number(2));
    ffmpegProcess->addArgument("-i");
    ffmpegProcess->addArgument("E:\\films\\jp.mkv");
    ffmpegProcess->addArgument("-i");
    ffmpegProcess->addArgument("E:\\films\\palette.png");
    ffmpegProcess->addArgument("-lavfi");
    ffmpegProcess->addArgument("fps=15,scale=320:-1:flags=lanczos [x]; [x][1:v] paletteuse");
    ffmpegProcess->addArgument("-y");
    ffmpegProcess->addArgument("E:\\films\\anim.gif");
}
void GifGenerator::generateGif()
{
    ffmpegProcess->start();

}
void GifGenerator::ffmpegConsole(QByteArray ba)
{
    qDebug()<<QString(ba);
    if(ba.contains("frame"))
        qDebug("Processing...");

}

void GifGenerator::emitProcessFinished(int ec)
{
    emit ffmpegexit(ec);
    qDebug()<<"GifGenerator Exit code :"<<QString::number(ec);
}
