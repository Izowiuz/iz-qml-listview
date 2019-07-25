#ifndef IZLISTVIEW_LISTVIEWIMPL_H
#define IZLISTVIEW_LISTVIEWIMPL_H

#include <QQmlListProperty>
#include <QQuickItem>

class QQmlPropertyMap;

namespace IzModels
{
	class AbstractItemModel;
}   // namespace IzModels

namespace IzListView
{
	class Filter;
	class FiltersModel;

	class ListViewImpl : public QQuickItem
	{
		Q_OBJECT
		Q_DISABLE_COPY(ListViewImpl)

		// model of the view
		Q_PROPERTY(QObject* model READ model WRITE setModel NOTIFY modelChanged FINAL)

		// row height provider
		Q_PROPERTY(QJSValue rowHeightProvider READ rowHeightProvider WRITE setRowHeightProvider NOTIFY rowHeightProviderChanged FINAL)

		// row delegate provider
		Q_PROPERTY(QJSValue rowDelegateProvider READ rowDelegateProvider WRITE setRowDelegateProvider NOTIFY rowDelegateProviderChanged FINAL)

		// row color provider
		Q_PROPERTY(QJSValue rowColorProvider READ rowColorProvider WRITE setRowColorProvider NOTIFY rowColorProviderChanged FINAL)

		// listview filters
		Q_PROPERTY(QQmlListProperty<IzListView::Filter> filters READ filters CONSTANT)

		// filters model
		Q_PROPERTY(IzListView::FiltersModel* filtersModel MEMBER m_filters CONSTANT FINAL)

		// current row of the view
		// always avaiable property iz_rowValid secifies if row is valid [view has halid selection] or not
		Q_PROPERTY(QQmlPropertyMap* currentRow READ currentRow NOTIFY currentRowChanged FINAL)

	public:
		// ctor
		explicit ListViewImpl(QQuickItem* parent = nullptr);

		// dtor
		~ListViewImpl() = default;

		// m_model getter / setter
		QObject* model() const;
		void setModel(QObject* model);

		// m_rowHeightProvider getter / setter
		QJSValue rowHeightProvider() const;
		void setRowHeightProvider(const QJSValue& callback);

		// m_rowDelegateProvider getter / setter
		QJSValue rowDelegateProvider() const;
		void setRowDelegateProvider(const QJSValue& callback);

		// m_rowColorProvider getter / setter
		QJSValue rowColorProvider() const;
		void setRowColorProvider(const QJSValue& callback);

		// filters getter
		QQmlListProperty<Filter> filters();

		// sets current row and its values according to the passed view index
		// WARNING: sourceModelIndex needs to be an index from source model
		Q_INVOKABLE void setCurrentRow(int sourceModelIndex);

		// returns data for given source index role
		// returns invalid QVariant if sourceModelIndex is invalid or given roleName is invalid
		Q_INVOKABLE QVariant roleData(int sourceModelIndex, const QString& roleName) const;

		// current row getter
		QQmlPropertyMap* currentRow() const;

		// QQmlParserStatus interface start

		void classBegin() override;

		// QQmlParserStatus interface end

	private:
		// filters modifiers start

		void appendFilter(Filter* filter);
		int filtersCount() const;
		Filter* filter(int index) const;
		void clearFilters();

		// filters modifiers end

		// model of the view
		IzModels::AbstractItemModel* m_model;

		// row height provider
		QJSValue m_rowHeightProvider;

		// row delegate provider
		QJSValue m_rowDelegateProvider;

		// row color provider
		QJSValue m_rowColorProvider;

		// filters model
		FiltersModel* m_filters;

		// internal current index of the view
		// WARNING: it needs to be set from QML side
		int m_currentIndex{ -1 };

		// current row data
		QQmlPropertyMap* m_currentRow{ nullptr };

		// null current row
		QQmlPropertyMap* m_nullCurrentRow;

	signals:
		// Q_PROPERTY *Changed signals
		void modelChanged();
		void rowHeightProviderChanged();
		void rowDelegateProviderChanged();
		void rowColorProviderChanged();
		void currentRowChanged();
	};
}   // namespace IzListView

#endif   // IZLISTVIEW_LISTVIEWIMPL_H
