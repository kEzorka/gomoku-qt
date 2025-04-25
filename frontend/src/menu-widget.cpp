#include "../include/menu-widget.h"

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent)
{
    // Основной вертикальный layout для меню
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Заголовок меню
    QLabel* title = new QLabel("Гомоку", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #2a5885;");
    mainLayout->addWidget(title);

    // Метка для выбора уровня сложности
    QLabel* difficultyLabel = new QLabel("Выберите уровень сложности:", this);
    mainLayout->addWidget(difficultyLabel);

    // Выпадающий список для уровня сложности.
    difficultyCombo = new QComboBox(this);
    // Значения сложности: лёгкий (глубина 2), средний (глубина 3), трудный (глубина 4)
    difficultyCombo->addItem("Лёгкий", 2);
    difficultyCombo->addItem("Средний", 3);
    difficultyCombo->addItem("Трудный", 4);
    mainLayout->addWidget(difficultyCombo);

    // Выбор режима игры
    QLabel* modeLabel = new QLabel("Выберите режим игры:", this);
    mainLayout->addWidget(modeLabel);
    rbPlayerVsBot = new QRadioButton("Игрок против Бота", this);
    rbBotVsBot   = new QRadioButton("Бот против Бота", this);
    rbPlayerVsBot->setChecked(true); // по умолчанию "Игрок против Бота"
    mainLayout->addWidget(rbPlayerVsBot);
    mainLayout->addWidget(rbBotVsBot);

    // Кнопка "Начать игру"
    btnStart = new QPushButton("Начать игру", this);
    mainLayout->addWidget(btnStart);
    connect(btnStart, &QPushButton::clicked, this, &MenuWidget::onStartButtonClicked);

    // Стилизация элементов меню (голубой фон, зелёные и сдержанные цвета)
    this->setStyleSheet("QWidget { background-color: #e0f7fa; } "
                        "QPushButton { background-color: #4caf50; color: white; border-radius: 5px; padding: 10px; } "
                        "QComboBox { padding: 5px; } "
                        "QRadioButton { padding: 5px; } "
                        "QLabel { color: #2a5885; }");

    // Фиксированный размер меню, совпадающий с игровым полем (600x600)
    this->setFixedSize(600, 600);
}

void MenuWidget::onStartButtonClicked()
{
    bool playerVsBot = rbPlayerVsBot->isChecked();
    int difficulty = difficultyCombo->currentData().toInt();
    // Если выбран режим "Бот против Бота", устанавливаем среднюю сложность (глубина 3)
    if (rbBotVsBot->isChecked()) {
        difficulty = 3;
    }
    emit startGameRequested(playerVsBot, difficulty);
}
