#include "FiltersModel.h"

#include <QDebug>

#include "IzModels/AbstractItemModel.h"

#include "Filter.h"

IzListView::FiltersModel::FiltersModel(QObject* parent)
	: QAbstractItemModel(parent)
{
}

QHash<int, QByteArray> IzListView::FiltersModel::roleNames() const
{
	QHash<int, QByteArray> rn;
	rn.insert(static_cast<int>(FiltersModelRoles::Role), QByteArrayLiteral("role"));
	rn.insert(static_cast<int>(FiltersModelRoles::DisplayRole), QByteArrayLiteral("displayRole"));
	rn.insert(static_cast<int>(FiltersModelRoles::FilterModel), QByteArrayLiteral("filterModel"));
	rn.insert(static_cast<int>(FiltersModelRoles::Type), QByteArrayLiteral("type"));
	rn.insert(static_cast<int>(FiltersModelRoles::Placeholder), QByteArrayLiteral("placeholder"));
	rn.insert(static_cast<int>(FiltersModelRoles::FilterValue), QByteArrayLiteral("filterValue"));
	rn.insert(static_cast<int>(FiltersModelRoles::IsFiltered), QByteArrayLiteral("isFiltered"));
	return rn;
}

QModelIndex IzListView::FiltersModel::index(int row, int column, const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return createIndex(row, column);
}

QModelIndex IzListView::FiltersModel::parent(const QModelIndex& index) const
{
	Q_UNUSED(index)
	return {};
}

int IzListView::FiltersModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return m_filters.size();
}

int IzListView::FiltersModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return 1;
}

QVariant IzListView::FiltersModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) {
		return {};
	}
	if (index.row() > static_cast<int>(m_filters.size())) {
		return {};
	}
	switch (static_cast<FiltersModelRoles>(role)) {
	case FiltersModelRoles::Type:
		return static_cast<int>(m_filters[index.row()]->type());
	case FiltersModelRoles::FilterModel:
		return QVariant::fromValue(m_filters[index.row()]->model());
	case FiltersModelRoles::Role:
		return m_filters[index.row()]->role();
	case FiltersModelRoles::DisplayRole:
		return m_filters[index.row()]->displayRole();
	case FiltersModelRoles::Placeholder:
		return m_filters[index.row()]->placeholderText();
	case FiltersModelRoles::FilterValue:
		return m_filters[index.row()]->filterValue();
	case FiltersModelRoles::IsFiltered:
		return !m_filters[index.row()]->filterValue().isEmpty();
	}
	return {};
}

bool IzListView::FiltersModel::empty() const
{
	return m_filters.empty();
}

IzListView::Filter* IzListView::FiltersModel::getFilter(int index) const
{
	if (index > static_cast<int>(m_filters.size())) {
		return {};
	}
	return m_filters[index];
}

void IzListView::FiltersModel::addFilter(IzListView::Filter* filter)
{
	if (filter == nullptr) {
		return;
	}
	auto nextIndex = m_filters.size();
	beginInsertRows({}, nextIndex, nextIndex);
	filter->setParent(this);
	connect(filter, &IzListView::Filter::stateChanged, this, [this](int index) {
		emit dataChanged(this->index(index, 0), this->index(index, 0), {});
	});
	filter->setInternalIndex(nextIndex);
	m_filters.push_back(filter);
	endInsertRows();
	emit countChanged();
}

void IzListView::FiltersModel::clearFilters()
{
	beginResetModel();
	qDeleteAll(m_filters);
	m_filters.clear();
	endResetModel();
	emit countChanged();
}

bool IzListView::FiltersModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid()) {
		return false;
	}
	if (role == static_cast<int>(FiltersModelRoles::FilterValue) && m_filters[index.row()]->filterValue() != value.toString()) {
		m_filters[index.row()]->setFilterValue(value.toString());
		emit dataChanged(this->index(index.row(), 0), this->index(index.row(), 0), { static_cast<int>(FiltersModelRoles::FilterValue), static_cast<int>(FiltersModelRoles::IsFiltered) });
		return true;
	} else if (role == static_cast<int>(FiltersModelRoles::FilterModel)) {
		m_filters[index.row()]->setModel(value.value<IzModels::AbstractItemModel*>());
		emit dataChanged(this->index(index.row(), 0), this->index(index.row(), 0), { static_cast<int>(FiltersModelRoles::FilterModel) });
		return true;
	} else if (role == static_cast<int>(FiltersModelRoles::DisplayRole)) {
		m_filters[index.row()]->setDisplayRole(value.toString());
		emit dataChanged(this->index(index.row(), 0), this->index(index.row(), 0), { static_cast<int>(FiltersModelRoles::DisplayRole) });
		return true;
	}
	return false;
}
