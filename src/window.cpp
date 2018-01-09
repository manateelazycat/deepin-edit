/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "window.h"
#include <DTitlebar>
#include <QLabel>
#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QSvgWidget>
#include "dthememanager.h"
#include "utils.h"

Window::Window(DMainWindow *parent) : DMainWindow(parent)
{
    DThemeManager::instance()->setTheme("dark");

    installEventFilter(this);   // add event filter

    layoutWidget = new QWidget();
    layout = new QStackedLayout(layoutWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setCentralWidget(layoutWidget);

    tabbar = new Tabbar();
    
    this->titlebar()->setCustomWidget(tabbar, Qt::AlignVCenter, false);
    this->titlebar()->setSeparatorVisible(true);
    
    connect(tabbar, SIGNAL(doubleClicked()), this->titlebar(), SIGNAL(doubleClicked()), Qt::QueuedConnection);
    
    Utils::applyQss(this, "main.qss");
}

Window::~Window()
{
    // We don't need clean pointers because application has exit here.
}

void Window::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->modifiers() == Qt::ControlModifier) {
        if (keyEvent->key() == Qt::Key_N) {
            tabbar->addTab("Bob Dylan");
        }
    }
}

int Window::fileIsInTabs(QString file)
{
    if (tabMap.contains(file)) {
        return tabMap[file];
    }
    
    return -1;
}

void Window::addTab(QString file)
{
    if (!tabMap.contains(file)) {
        tabbar->addTab(QFileInfo(file).fileName());
        tabMap[file] = tabbar->currentIndex() + 1;
        
        if (!editorMap.contains(file)) {
            Editor *editor = new Editor();
            editor->loadFile(file);
            
            editorMap[file] = editor;
            
            layout->addWidget(editor);
            layout->setCurrentIndex(layout->count() - 1);
        }
    }
}
