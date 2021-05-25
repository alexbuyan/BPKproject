#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void update_chats();

    void update_messages(std::size_t chat_id);

    Ui::MainWindow *get_ui() const;

    std::unordered_map<QListWidgetItem *, std::size_t> icon_to_chat_id;

signals:

    void authWin();

private slots:

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

signals:

    void messWin();

private:
    Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
