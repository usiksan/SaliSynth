#ifndef QMLMIDITRACK_H
#define QMLMIDITRACK_H

#include <QAbstractListModel>

class QmlMidiTrack : public QAbstractListModel
  {
    Q_OBJECT
  public:
    QmlMidiTrack();

    void beginReadTrack();
    void endReadTrack();

    // QAbstractItemModel interface
  public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
  };

#endif // QMLMIDITRACK_H
