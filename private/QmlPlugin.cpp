#include "IzListView/QmlPlugin.h"

#include <QQmlPropertyMap>
#include <QtQml>

#include "IzModels/AbstractItemModel.h"
#include "IzModels/NullModel.h"
#include "IzModels/ProxyModel.h"
#include "IzModels/StringListModel.h"
#include "IzModels/VariantListModel.h"

#include "Filter.h"
#include "FiltersModel.h"
#include "ListViewImpl.h"

void IzListView::QmlPlugin::registerTypes(const char* uri)
{
	Q_ASSERT(uri == QLatin1String("IzListView"));

	qRegisterMetaType<QQmlPropertyMap*>("QQmlPropertyMap*");

	qRegisterMetaType<IzModels::AbstractItemModel*>("IzModels::AbstractItemModel*");
	qRegisterMetaType<IzModels::ProxyModel*>("IzModels::ProxyModel*");
	qRegisterMetaType<IzModels::StringListModel*>("IzModels::StringListModel*");
	qRegisterMetaType<IzModels::VariantListModel*>("IzModels::VariantListModel*");
	qRegisterMetaType<IzListView::FiltersModel*>("IzListView::FiltersModel*");

	qmlRegisterType(QUrl(QStringLiteral("qrc:/include/IzListView/QML/IzListView.qml")), "IzListView", 1, 0, "IzListView");
	qmlRegisterType<ListViewImpl>(uri, 1, 0, "IzListViewImpl");
	qmlRegisterType<IzModels::ProxyModel>(uri, 1, 0, "IzProxyModel");
	qmlRegisterType<IzModels::StringListModel>(uri, 1, 0, "IzStringListModel");
	qmlRegisterType<IzModels::VariantListModel>(uri, 1, 0, "IzVariantListModel");
	qmlRegisterType<Filter>(uri, 1, 0, "IzListViewFilter");
}
