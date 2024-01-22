/* This file was created by Kureii (Tomas Adamek)
* Date created: 21. 01. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/gui/copy_to_clipboard.h"
#include <QClipboard>
#include <QGuiApplication>

//================================= Namespace ==================================
namespace rain_text::gui {

//================================= Public method ==============================
CopyToClipboard::CopyToClipboard(QObject* parent) : QObject(parent) {}

void CopyToClipboard::copyToClipboard(const QString &text) {
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->setText(text);
}
//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text::gui