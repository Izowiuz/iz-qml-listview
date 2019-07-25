#include "ListViewImpl.h"

#include <QDebug>
#include <QQmlEngine>
#include <QQmlPropertyMap>

#include "IzModels/AbstractItemModel.h"
#include "IzModels/NullModel.h"

#include "Filter.h"
#include "FiltersModel.h"

#include "IzModels/AbstractItemModel.h"

IzListView::ListViewImpl::ListViewImpl(QQuickItem* parent)
	: QQuickItem(parent)
	, m_model(new IzModels::NullModel(this))
	, m_filters(new FiltersModel(this))
	, m_nullCurrentRow(new QQmlPropertyMap(this))
{
	m_nullCurrentRow->insert(QStringLiteral("iz_rowValid"), false);
}

QObject* IzListView::ListViewImpl::model() const
{
	return m_model;
}

void IzListView::ListViewImpl::setModel(QObject* model)
{
	if (m_model != model) {
		if (qobject_cast<IzModels::NullModel*>(m_model) != nullptr) {
			m_model->deleteLater();
		}

		if (qobject_cast<IzModels::AbstractItemModel*>(model) == nullptr) {
			qCritical() << "Object:" << model << "is not a subclass of IzModels::AbstractItemModel. Model will be invalid.";
			m_model = new IzModels::NullModel(this);
		} else {
			m_model = qobject_cast<IzModels::AbstractItemModel*>(model);

			// model connects
			connect(m_model, &IzModels::AbstractItemModel::roleNamesCached, this, [this]() {
				// TODO: odpalać to tylko wtedy, gdy cachedRoleNames się faktycznie zmienią
				delete m_currentRow;
				m_currentRow = new QQmlPropertyMap(this);
				for (auto it = m_model->cachedRoleNamesCBegin(); it != m_model->cachedRoleNamesCEnd(); ++it) {
					m_currentRow->insert(*it, {});
				}

				m_currentRow->insert(QStringLiteral("iz_rowValid"), false);
			});
			connect(m_model, &IzModels::AbstractItemModel::modelReset, this, &ListViewImpl::currentRowChanged);
		}
		emit modelChanged();
	}
}

QJSValue IzListView::ListViewImpl::rowHeightProvider() const
{
	return m_rowHeightProvider;
}

void IzListView::ListViewImpl::setRowHeightProvider(const QJSValue& callback)
{
	if (!callback.isCallable()) {
		qWarning() << "rowHeightProvider must be a callable function.";
		return;
	}

	m_rowHeightProvider = callback;
	emit rowHeightProviderChanged();
}

QJSValue IzListView::ListViewImpl::rowDelegateProvider() const
{
	return m_rowDelegateProvider;
}

void IzListView::ListViewImpl::setRowDelegateProvider(const QJSValue& callback)
{
	if (!callback.isCallable()) {
		qWarning() << "rowDelegateProvider must be a callable function.";
		return;
	}

	m_rowDelegateProvider = callback;
	emit rowDelegateProviderChanged();
}

QJSValue IzListView::ListViewImpl::rowColorProvider() const
{
	return m_rowColorProvider;
}

void IzListView::ListViewImpl::setRowColorProvider(const QJSValue& callback)
{
	if (!callback.isCallable()) {
		qWarning() << "rowColorProvider must be a callable function.";
		return;
	}

	m_rowColorProvider = callback;
	emit rowColorProviderChanged();
}

QQmlListProperty<IzListView::Filter> IzListView::ListViewImpl::filters()
{
	return { this,
			 nullptr,
			 // append
			 [](QQmlListProperty<Filter>* filters, Filter* filter) {
				 auto* c = qobject_cast<ListViewImpl*>(filters->object);
				 c->appendFilter(filter);
			 },
			 // count
			 [](QQmlListProperty<Filter>* filters) -> int {
				 auto* c = qobject_cast<ListViewImpl*>(filters->object);
				 return c->filtersCount();
			 },
			 // get
			 [](QQmlListProperty<Filter>* filters, int index) -> Filter* {
				 auto* c = qobject_cast<ListViewImpl*>(filters->object);
				 return c->filter(index);
			 },
			 // clear
			 [](QQmlListProperty<Filter>* filters) {
				 auto* c = qobject_cast<ListViewImpl*>(filters->object);
				 c->clearFilters();
			 } };
}

void IzListView::ListViewImpl::setCurrentRow(int sourceModelIndex)
{
	if (sourceModelIndex > m_model->rowCount()) {
		m_currentIndex = -1;
	} else {
		m_currentIndex = sourceModelIndex;
		if (m_currentRow != nullptr) {
			for (auto it = m_model->cachedRoleNamesCBegin(); it != m_model->cachedRoleNamesCEnd(); ++it) {
				m_currentRow->insert(*it, m_model->data(m_model->index(sourceModelIndex, 0), m_model->roleNameToRole(*it)));
			}
			m_currentRow->insert(QStringLiteral("iz_rowValid"), true);
		}
	}

	emit currentRowChanged();
}

QVariant IzListView::ListViewImpl::roleData(int sourceModelIndex, const QString &roleName) const
{
	return m_model->data(m_model->index(sourceModelIndex, 0), m_model->roleNameToRole(roleName));
}

QQmlPropertyMap* IzListView::ListViewImpl::currentRow() const
{
	if (m_currentIndex == -1 || m_currentRow == nullptr) {
		return m_nullCurrentRow;
	}

	return m_currentRow;
}

void IzListView::ListViewImpl::classBegin()
{
	auto engine = qmlEngine(this);

	if (engine != nullptr) {
		m_rowHeightProvider   = engine->evaluate(QStringLiteral("( function(row, model) { return 40; } )"));
		m_rowDelegateProvider = engine->evaluate(QStringLiteral("( function(row, model) { return \"\"; } )"));
		m_rowColorProvider    = engine->evaluate(QStringLiteral("( function(row, model) { return \"transparent\"; } )"));
	} else {
		qWarning() << "Got invalid qmlEngine hadler. Providers will be undefined.";
	}
}

void IzListView::ListViewImpl::appendFilter(IzListView::Filter* filter)
{
	if (filter == nullptr) {
		return;
	}

	QQmlEngine::setObjectOwnership(filter, QQmlEngine::CppOwnership);
	m_filters->addFilter(filter);
}

int IzListView::ListViewImpl::filtersCount() const
{
	return m_filters->rowCount();
}

IzListView::Filter* IzListView::ListViewImpl::filter(int index) const
{
	return m_filters->getFilter(index);
}

void IzListView::ListViewImpl::clearFilters()
{
	m_filters->clearFilters();
}
