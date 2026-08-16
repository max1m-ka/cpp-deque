#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include <algorithm>

static std::deque<std::string> tea {
    "Чай Лунцзин",
    "Эрл Грей",
    "Сенча",
    "Пуэр",
    "Дарджилинг",
    "Ассам",
    "Матча",
    "Ганпаудер",
    "Оолонг",
    "Лапсанг Сушонг"
};

static std::deque<std::string> cakes {
    "Красный бархат",
    "Наполеон",
    "Медовик",
    "Тирамису",
    "Прага",
    "Чизкейк",
    "Захер",
    "Эстерхази",
    "Морковный торт",
    "Чёрный лес",
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , random_gen(std::random_device{}())
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ApplyModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ApplyModel()
{
    ui->list_widget->clear();

    for (size_t i = 0; i < vector_model_.items.size(); ++i) {
        QString item =
            QString::number(i) +
            ": " +
            QString::fromStdString(vector_model_.items[i]);

        ui->list_widget->addItem(item);
    }

    ui->list_widget->addItem("end");

    ui->txt_size->setText(
        QString::number(vector_model_.items.size())
        );

    const bool has_elements = !vector_model_.items.empty();

    ui->pop_back->setEnabled(has_elements);
    ui->pop_front->setEnabled(has_elements);

    ApplyIterator();
}

void MainWindow::ApplyIterator()
{
    if (vector_model_.iterator == vector_model_.items.end()) {
        ui->txt_elem_content->clear();

        ui->Edit->setDisabled(true);
        ui->erase->setDisabled(true);
        ui->plus->setDisabled(true);
    }
    else {
        ui->txt_elem_content->setText(
            QString::fromStdString(*vector_model_.iterator)
            );

        ui->Edit->setEnabled(true);
        ui->erase->setEnabled(true);
        ui->plus->setEnabled(true);
    }

    if (vector_model_.iterator == vector_model_.items.begin()) {
        ui->minus->setDisabled(true);
    }
    else {
        ui->minus->setEnabled(true);
    }

    const auto distance =
        std::distance(
            vector_model_.items.begin(),
            vector_model_.iterator
            );

    ui->list_widget->setCurrentRow(
        static_cast<int>(distance)
        );
}

void MainWindow::on_push_back_clicked()
{
    const QString text = ui->txt_elem_content->text();

    if (!text.isEmpty()) {
        vector_model_.items.push_back(text.toStdString());

        vector_model_.iterator = vector_model_.items.begin();

        ApplyModel();
    }
}

void MainWindow::on_pop_back_clicked()
{
    if (!vector_model_.items.empty()) {
        vector_model_.items.pop_back();

        vector_model_.iterator = vector_model_.items.begin();

        ApplyModel();
    }
}

void MainWindow::on_push_front_clicked()
{
    vector_model_.items.push_front(
        ui->txt_elem_content->text().toStdString()
        );

    vector_model_.iterator = vector_model_.items.begin();

    ApplyModel();
}

void MainWindow::on_pop_front_clicked()
{
    if (!vector_model_.items.empty()) {
        vector_model_.items.pop_front();

        vector_model_.iterator = vector_model_.items.begin();

        ApplyModel();
    }
}

void MainWindow::on_clear_clicked()
{
    vector_model_.items.clear();

    vector_model_.iterator = vector_model_.items.begin();

    ApplyModel();
}

void MainWindow::on_tea_clicked()
{
    vector_model_.items = tea;

    vector_model_.iterator = vector_model_.items.begin();

    ApplyModel();
}

void MainWindow::on_cakes_clicked()
{
    vector_model_.items = cakes;

    vector_model_.iterator = vector_model_.items.begin();

    ApplyModel();
}

void MainWindow::on_begin_clicked()
{
    vector_model_.iterator =
        vector_model_.items.begin();

    ApplyIterator();
}

void MainWindow::on_end_clicked()
{
    vector_model_.iterator =
        vector_model_.items.end();

    ApplyIterator();
}

void MainWindow::on_plus_clicked()
{
    if (vector_model_.iterator != vector_model_.items.end()) {
        ++vector_model_.iterator;

        ApplyIterator();
    }
}

void MainWindow::on_minus_clicked()
{
    if (vector_model_.iterator != vector_model_.items.begin()) {
        --vector_model_.iterator;

        ApplyIterator();
    }
}

void MainWindow::on_list_widget_currentRowChanged(int currentRow)
{
    if (currentRow < 0) {
        return;
    }

    if (currentRow <
        static_cast<int>(vector_model_.items.size())) {

        vector_model_.iterator =
            vector_model_.items.begin() + currentRow;

        ApplyIterator();
    }
    else if (
        currentRow ==
        static_cast<int>(vector_model_.items.size())) {

        vector_model_.iterator =
            vector_model_.items.end();

        ApplyIterator();
    }
}

void MainWindow::on_Edit_clicked()
{
    if (vector_model_.iterator != vector_model_.items.end()) {

        const QString new_text =
            ui->txt_elem_content->text();

        if (!new_text.isEmpty()) {
            *vector_model_.iterator =
                new_text.toStdString();

            ApplyModel();
        }
    }
}

void MainWindow::on_insert_clicked()
{
    const QString new_text =
        ui->txt_elem_content->text();

    if (!new_text.isEmpty()) {

        vector_model_.items.insert(
            vector_model_.iterator,
            new_text.toStdString()
            );

        vector_model_.iterator =
            vector_model_.items.begin();

        ApplyModel();
    }
}

void MainWindow::on_erase_clicked()
{
    if (vector_model_.iterator != vector_model_.items.end()) {

        vector_model_.items.erase(
            vector_model_.iterator
            );

        vector_model_.iterator =
            vector_model_.items.begin();

        ApplyModel();
    }
}

void MainWindow::on_btn_resize_clicked()
{
    const int value =
        ui->txt_size->text().toInt();

    vector_model_.items.resize(value);

    vector_model_.iterator =
        vector_model_.items.begin();

    ApplyModel();
}

void MainWindow::on_pbn_count_clicked()
{
    const std::string searchText =
        ui->le_count->text().toStdString();

    const auto count =
        std::count(
            vector_model_.items.begin(),
            vector_model_.items.end(),
            searchText
            );

    ui->lbl_count->setText(
        QString::number(count)
        );
}

void MainWindow::on_pbn_find_clicked()
{
    const std::string findText =
        ui->txt_elem_content->text().toStdString();

    vector_model_.iterator =
        std::find(
            vector_model_.items.begin(),
            vector_model_.items.end(),
            findText
            );

    ApplyModel();
}

void MainWindow::on_pbn_min_clicked()
{
    vector_model_.iterator =
        std::min_element(
            vector_model_.items.begin(),
            vector_model_.items.end()
            );

    ApplyModel();
}

void MainWindow::on_pbn_max_clicked()
{
    vector_model_.iterator =
        std::max_element(
            vector_model_.items.begin(),
            vector_model_.items.end()
            );

    ApplyModel();
}

void MainWindow::on_btn_sort_clicked()
{
    std::sort(
        vector_model_.items.begin(),
        vector_model_.items.end()
        );

    vector_model_.iterator =
        vector_model_.items.begin();

    ApplyModel();
}

void MainWindow::on_btn_sOrT_clicked()
{
    std::sort(
        vector_model_.items.begin(),
        vector_model_.items.end(),
        [](const std::string& a, const std::string& b) {
            return QString::compare(
                       QString::fromStdString(a),
                       QString::fromStdString(b),
                       Qt::CaseInsensitive
                       ) < 0;
        }
        );

    vector_model_.iterator =
        vector_model_.items.begin();

    ApplyModel();
}

void MainWindow::on_btn_unique_clicked()
{
    if (std::is_sorted(
            vector_model_.items.begin(),
            vector_model_.items.end())) {

        const auto last =
            std::unique(
                vector_model_.items.begin(),
                vector_model_.items.end()
                );

        vector_model_.items.erase(
            last,
            vector_model_.items.end()
            );

        vector_model_.iterator =
            vector_model_.items.begin();
    }

    ApplyModel();
}

void MainWindow::on_btn_reverse_clicked()
{
    std::reverse(
        vector_model_.items.begin(),
        vector_model_.items.end()
        );

    ApplyModel();
}

void MainWindow::SetRandomGen(
    const std::mt19937& random_gen)
{
    this->random_gen = random_gen;
}

void MainWindow::on_btn_shuffle_clicked()
{
    std::shuffle(
        vector_model_.items.begin(),
        vector_model_.items.end(),
        random_gen
        );

    ApplyModel();
}

void MainWindow::on_lower_bound_clicked()
{
    if (std::is_sorted(
            vector_model_.items.begin(),
            vector_model_.items.end())) {

        vector_model_.iterator =
            std::lower_bound(
                vector_model_.items.begin(),
                vector_model_.items.end(),
                ui->txt_elem_content->text().toStdString()
                );

        ApplyModel();
    }
}

void MainWindow::on_upper_bound_clicked()
{
    if (std::is_sorted(
            vector_model_.items.begin(),
            vector_model_.items.end())) {

        vector_model_.iterator =
            std::upper_bound(
                vector_model_.items.begin(),
                vector_model_.items.end(),
                ui->txt_elem_content->text().toStdString()
                );

        ApplyModel();
    }
}
