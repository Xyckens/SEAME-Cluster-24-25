#include "../includes/SettingsWidget.h"


/**
 * @brief Constructs a SettingsWidget with the specified parameters.
 *
 * @param parent The parent widget.
 * @param event Reference to the EventManager for handling user actions.
 * @param x The x-coordinate of the widget's position (default is 0).
 * @param y The y-coordinate of the widget's position (default is 0).
 * @param width The width of the widget (default is 0).
 * @param height The height of the widget (default is 0).
 */
SettingsWidget::SettingsWidget(QWidget *parent, EventManager &event, int x, int y, int width, int height)
    : QWidget(parent), expanded(false), event(event)
{
    setGeometry(x, y, width, height);
    main_icon = new QPushButton(this);
    main_icon->setFixedSize(50, 50);
    main_icon->setStyleSheet("border: none; "
                            "background-image: url('/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/settings.png'); "
                            "background-repeat: no-repeat; "
                            "background-position: center;"
                            );

    // Initialize close icon with an image
    closeIcon = new QPushButton(this);
    closeIcon->setFixedSize(50, 50);
    closeIcon->setStyleSheet("border: none; "
                             "background-image: url('/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/settings.png'); "
                             "background-repeat: no-repeat; " 
                             "background-position: center;");

    themes_icon = new QPushButton(this);
    themes_icon->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    themes_icon->setStyleSheet(
        "QPushButton {"
        "   border: none;"
        "   background: none;"
        "}"
        "QPushButton::icon {"
        "   margin-bottom: 50px;"
        "}"
        "QPushButton {"
        "   text-align: center;"
        "}"
        );
    themes_icon->setIcon(QIcon("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/brush.png"));
    themes_icon->setIconSize(QSize(50, 50));
    themes_icon->setText("Change Themes");

    change_unit_icon = new QPushButton(this);
    change_unit_icon->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    change_unit_icon->setStyleSheet(
        "QPushButton {"
        "   border: none;"
        "   background: none;"
        "}"
        "QPushButton::icon {"
        "   margin-bottom: 50px;"
        "}"
        "QPushButton {"
        "   text-align: center;"
        "}"
        );
    change_unit_icon->setIcon(QIcon("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/convert.png"));
    change_unit_icon->setIconSize(QSize(50, 50));
    change_unit_icon->setText("Change Units");


    collapsed_widget = new QWidget(this);
    QVBoxLayout *collapsedLayout = new QVBoxLayout(collapsed_widget);
    collapsedLayout->addWidget(main_icon);

    collapsedLayout->setContentsMargins(0, 0, 0, 0);
    collapsedLayout->setSpacing(0);

    expanded_widget = new QWidget(this);
    QVBoxLayout *expandedLayout = new QVBoxLayout(expanded_widget);
    expandedLayout->addWidget(themes_icon);
    expandedLayout->addWidget(change_unit_icon);
    expandedLayout->addWidget(close_icon);

    expandedLayout->setContentsMargins(0, 0, 0, 0);
    expandedLayout->setSpacing(10);

    stacked_widget = new QStackedWidget(this);
    stacked_widget->addWidget(collapsed_widget);
    stacked_widget->addWidget(expanded_widget);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stacked_widget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);


    setLayout(mainLayout);
    setFixedSize(140, 120); // Collapsed size


    connect(main_icon, &QPushButton::clicked, this, &SettingsWidget::toggleExpand);
    connect(close_icon, &QPushButton::clicked, this, &SettingsWidget::toggleExpand);
    connect(change_unit_icon, &QPushButton::clicked, stats, &event->changeUnits());
    connect(themes_icon, &QPushButton::clicked, stats, &event->changeColors());
}

/**
 * @brief Toggles the widget between collapsed and expanded states.
 *
 * This slot is triggered when the user clicks the main icon to expand or collapse the settings menu.
 */
void SettingsWidget::toggleExpand()
{
    setExpanded(!expanded);
}

/**
 * @brief Expands or collapses the settings menu.
 *
 * This method updates the widget's state and UI to reflect the expanded or collapsed state.
 *
 * @param expand If true, the widget is expanded; if false, it is collapsed.
 */
void SettingsWidget::setExpanded(bool expand)
{
    expanded = expand;

    if (expanded) {
        stacked_widget->setCurrentWidget(expanded_widget);
        setMinimumSize(140, 200);
        setMaximumSize(140, 200);
    } else {
        stacked_widget->setCurrentWidget(collapsed_widget);
        setMinimumSize(140, 120);
        setMaximumSize(140, 120);
    }
}

/**
 * @brief Handles mouse press events to detect clicks outside the widget.
 *
 * This method is overridden from QWidget to detect clicks outside the widget and collapse
 * the settings menu if it is expanded.
 *
 * @param event The mouse event.
 */
void SettingsWidget::mousePressEvent(QMouseEvent *event)
{
    if (expanded && isClickOutside(event->pos())) {
        setExpanded(false);
    }

    // Call the base class implementation to handle other events
    QWidget::mousePressEvent(event);
}

/**
 * @brief Checks if a click occurred outside the widget.
 *
 * This method determines whether a mouse click occurred outside the bounds of the widget.
 *
 * @param pos The position of the mouse click.
 * @return true if the click was outside the widget, false otherwise.
 */
bool SettingsWidget::isClickOutside(const QPoint &pos) const
{
    // Check if the click position is outside the widget's rectangle
    return !rect().contains(pos);
}
