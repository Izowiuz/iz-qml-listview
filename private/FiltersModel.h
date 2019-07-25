#ifndef IZLISTVIEW_FILTERSMODEL_H
#define IZLISTVIEW_FILTERSMODEL_H

#include <vector>

#include <QAbstractItemModel>

namespace IzListView
{
	class Filter;

	class FiltersModel : public QAbstractItemModel
	{
		Q_OBJECT
		Q_DISABLE_COPY(FiltersModel)

		Q_PROPERTY(int count READ rowCount NOTIFY countChanged FINAL)
		Q_PROPERTY(bool empty READ empty NOTIFY countChanged FINAL)

	public:
		enum class FiltersModelRoles : int {
			Role = Qt::UserRole,
			DisplayRole,
			FilterModel,
			Type,
			Placeholder,
			FilterValue,
			IsFiltered
		};

		// ctor
		explicit FiltersModel(QObject* parent = nullptr);

		// dtor
		~FiltersModel() = default;

		// QAbstractItemModel interface start

		QHash<int, QByteArray> roleNames() const override;
		QModelIndex index(int row, int column, const QModelIndex& parent = {}) const override;
		QModelIndex parent(const QModelIndex& index) const override;
		int rowCount(const QModelIndex& parent = {}) const override;
		int columnCount(const QModelIndex& parent = {}) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		bool setData(const QModelIndex& index, const QVariant& value, int role) override;

		// QAbstractItemModel interface start end

		// returns true if model is empty
		bool empty() const;

		// returns filter under given index
		Filter* getFilter(int index) const;

		// adds new filter
		void addFilter(Filter* filter);

		// clears all filters
		void clearFilters();

	private:
		// internal data
		std::vector<Filter*> m_filters;

	signals:
		// Q_PROPERTY *Changed signals
		void countChanged();
	};
}   // namespace IzListView

#endif   // IZLISTVIEW_FILTERSMODEL_H
