#include "Filter.h"

#include <QDebug>

#include "IzModels/AbstractItemModel.h"
#include "IzModels/NullModel.h"

IzListView::Filter::Filter(QObject* parent)
	: QObject(parent)
{
}

QString IzListView::Filter::role() const
{
	return m_role;
}

void IzListView::Filter::setRole(const QString& role)
{
	if (m_role != role) {
		m_role = role;
		emit roleChanged();
		if (m_componentInitialized) {
			emit stateChanged(m_internalIndex);
		}
	}
}

void IzListView::Filter::classBegin()
{
}

void IzListView::Filter::componentComplete()
{
	m_componentInitialized = true;
}

int IzListView::Filter::internalIndex() const
{
	return m_internalIndex;
}

void IzListView::Filter::setInternalIndex(int internalIndex)
{
	m_internalIndex = internalIndex;
}

QString IzListView::Filter::placeholderText() const
{
	return m_placeholderText;
}

void IzListView::Filter::setPlaceholderText(const QString& placeholder)
{
	if (m_placeholderText != placeholder) {
		m_placeholderText = placeholder;
		emit placeholderChanged();
		if (m_componentInitialized) {
			emit stateChanged(m_internalIndex);
		}
	}
}

QString IzListView::Filter::filterValue() const
{
	return m_filterValue;
}

void IzListView::Filter::setFilterValue(const QString& filterValue)
{
	m_filterValue = filterValue;
}

IzListView::Filter::Type IzListView::Filter::type() const
{
	return m_type;
}

void IzListView::Filter::setType(Type type)
{
	if (m_type != type) {
		m_type = type;
		emit typeChanged();
		if (m_componentInitialized) {
			emit stateChanged(m_internalIndex);
		}
	}
}

QObject* IzListView::Filter::model() const
{
	return m_model;
}

void IzListView::Filter::setModel(QObject* model)
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
		}

		emit modelChanged();
		if (m_componentInitialized) {
			emit stateChanged(m_internalIndex);
		}
	}
}

QString IzListView::Filter::displayRole() const
{
	return m_displayRole;
}

void IzListView::Filter::setDisplayRole(const QString& displayRole)
{
	if (m_displayRole != displayRole) {
		m_displayRole = displayRole;
		emit displayRoleChanged();
		if (m_componentInitialized) {
			emit stateChanged(m_internalIndex);
		}
	}
}
