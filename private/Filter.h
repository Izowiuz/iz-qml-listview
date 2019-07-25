#ifndef IZLISTVIEW_FILTER_H
#define IZLISTVIEW_FILTER_H

#include <QObject>
#include <QQmlParserStatus>

namespace IzModels
{
	class AbstractItemModel;
}   // namespace IzModels

namespace IzListView
{
	class Filter : public QObject, public QQmlParserStatus
	{
		Q_OBJECT
		Q_DISABLE_COPY(Filter)

		// type of this filter
		// currently supported are TextField, Completer and ComboBox
		Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged FINAL)

		// model role that this filter operates on
		Q_PROPERTY(QString role READ role WRITE setRole NOTIFY roleChanged FINAL)

		// model role that this filter operates on
		Q_PROPERTY(QString displayRole READ displayRole WRITE setDisplayRole NOTIFY displayRoleChanged FINAL)

		// placeholder for this filters
		Q_PROPERTY(QString placeholder READ placeholderText WRITE setPlaceholderText NOTIFY placeholderChanged FINAL)

		// data model for filter values, used when applicable
		Q_PROPERTY(QObject* model READ model WRITE setModel NOTIFY modelChanged FINAL)

	public:
		// type of this filter
		enum class Type : int {
			TextField = 0,
			Completer,
			ComboBox
		};
		Q_ENUMS(Type)

		// ctor
		explicit Filter(QObject* parent = nullptr);

		// dtor
		~Filter() = default;

		// QQmlParserStatus interface start

		void classBegin() override;
		void componentComplete() override;

		// QQmlParserStatus interface end

		// m_role getter / setter
		QString role() const;
		void setRole(const QString& role);

		// m_internalIndex getter / setter
		int internalIndex() const;
		void setInternalIndex(int internalIndex);

		// m_placeholderText getter / setter
		QString placeholderText() const;
		void setPlaceholderText(const QString& placeholder);

		// m_filterValue getter / setter
		QString filterValue() const;
		void setFilterValue(const QString& filterValue);

		// m_type getter / setter
		Type type() const;
		void setType(Type type);

		// m_model getter / setter
		QObject* model() const;
		void setModel(QObject* model);

		// m_displayRole getter / setter
		QString displayRole() const;
		void setDisplayRole(const QString& displayRole);

	private:
		// filter type
		Type m_type{ Type::TextField };

		// QML filter  role
		QString m_role;

		// QML display role
		QString m_displayRole;

		// filter placeholder text
		QString m_placeholderText;

		// filter value
		QString m_filterValue;

		// true if Page this page was initialized in qml engine
		bool m_componentInitialized{ false };

		// internal index of this page
		int m_internalIndex{ -1 };

		// optional filter model
		IzModels::AbstractItemModel* m_model{ nullptr };

	signals:
		// Q_PROPERTY *Changed signals
		void roleChanged();
		void placeholderChanged();
		void typeChanged();
		void modelChanged();
		void displayRoleChanged();

		// emited when state of this filter changes
		void stateChanged(int index);
	};
}   // namespace IzListView

#endif   // IZLISTVIEW_FILTER_H
