#ifdef QT_GUI
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_about.h"
#include "inttablewidget.hpp"
#include "../elfparser.hpp"
#include "../abstract_sectionheader.hpp"
#include "../abstract_programheader.hpp"

#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QClipboard>
#include <QAction>

#include <iostream>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_dialog(),
    m_tableItems(),
    m_treeItems(),
    m_copyAction(),
    m_parser()
{
    m_ui->setupUi(this);

    // create the copy action and apply signals as needed
    m_copyAction.reset(new QAction(tr("Copy"), this));
    m_copyAction->setShortcut(QKeySequence::Copy);
    connect(m_copyAction.get(), SIGNAL(triggered()), this, SLOT(overviewToClipboard()));

    // attach copy to widget
    m_ui->overviewTable->addAction(m_copyAction.get());
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::openFile()
{
    reset();
    m_parser.reset(new ELFParser());

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
    {
        if (dialog.selectedFiles().count() != 1)
        {
            return;
        }

        try
        {
            m_parser->parse(dialog.selectedFiles().at(0).toStdString());
            m_parser->evaluate();
        }
        catch (const std::exception& e)
        {
            std::string errorMessage("Loading Error: ");
            errorMessage.append(e.what());

            QMessageBox msgBox;
            msgBox.setText(errorMessage.c_str());
            msgBox.exec();
            return;
        }

        // LCD display
        m_ui->scoreDisplay->display(static_cast<int>(m_parser->getScore()));

        // Overview table
        QTableWidgetItem* tableItem = new QTableWidgetItem(QString(m_parser->getFilename().c_str()));
        m_ui->overviewTable->setItem(0, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_parser->getFileSize()).c_str()));
        m_ui->overviewTable->setItem(1, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getMD5().c_str()));
        m_ui->overviewTable->setItem(2, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getSha1().c_str()));
        m_ui->overviewTable->setItem(3, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getSha256().c_str()));
        m_ui->overviewTable->setItem(4, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getFamily().c_str()));
        m_ui->overviewTable->setItem(5, 0, tableItem);
        m_tableItems.push_back(tableItem);

        // elf header view
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getMagic().c_str()));
        m_ui->headerTable->setItem(0, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getClass().c_str()));
        m_ui->headerTable->setItem(1, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getEncoding().c_str()));
        m_ui->headerTable->setItem(2, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getFileVersion().c_str()));
        m_ui->headerTable->setItem(3, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getOSABI().c_str()));
        m_ui->headerTable->setItem(4, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getABIVersion().c_str()));
        m_ui->headerTable->setItem(5, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getType().c_str()));
        m_ui->headerTable->setItem(6, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getMachine().c_str()));
        m_ui->headerTable->setItem(7, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getVersion().c_str()));
        m_ui->headerTable->setItem(8, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getEntryPointString().c_str()));
        m_ui->headerTable->setItem(9, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_parser->getElfHeader().getProgramOffset()).c_str()));
        m_ui->headerTable->setItem(10, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_parser->getElfHeader().getSectionOffset()).c_str()));
        m_ui->headerTable->setItem(11, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getFlags().c_str()));
        m_ui->headerTable->setItem(12, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(m_parser->getElfHeader().getEHSize().c_str()));
        m_ui->headerTable->setItem(13, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_parser->getElfHeader().getProgramSize()).c_str()));
        m_ui->headerTable->setItem(14, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_parser->getElfHeader().getProgramCount()).c_str()));
        m_ui->headerTable->setItem(15, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_parser->getElfHeader().getSectionSize()).c_str()));
        m_ui->headerTable->setItem(16, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_parser->getElfHeader().getSectionCount()).c_str()));
        m_ui->headerTable->setItem(17, 0, tableItem);
        m_tableItems.push_back(tableItem);
        tableItem = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_parser->getElfHeader().getStringTableIndex()).c_str()));
        m_ui->headerTable->setItem(18, 0, tableItem);
        m_tableItems.push_back(tableItem);

        // sections table
        boost::uint32_t i = 0;
        const std::vector<AbstractSectionHeader>& sections(m_parser->getSectionHeaders().getSections());
        m_ui->sectionsTable->setRowCount(sections.size());
        m_ui->sectionsTable->setSortingEnabled(false);
        BOOST_FOREACH(const AbstractSectionHeader& section, sections)
        {
            tableItem = new IntWidgetItem(i);
            m_ui->sectionsTable->setItem(i, 0, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new QTableWidgetItem(QString(section.getName().c_str()));
            m_ui->sectionsTable->setItem(i, 1, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new QTableWidgetItem(QString(section.getTypeString().c_str()));
            m_ui->sectionsTable->setItem(i, 2, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new QTableWidgetItem(QString(section.getFlagsString().c_str()));
            m_ui->sectionsTable->setItem(i, 3, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(section.getVirtAddress(), true);
            m_ui->sectionsTable->setItem(i, 4, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(section.getPhysOffset(), false);
            m_ui->sectionsTable->setItem(i, 5, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(section.getSize());
            m_ui->sectionsTable->setItem(i, 6, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(section.getLink());
            m_ui->sectionsTable->setItem(i, 7, tableItem);
            m_tableItems.push_back(tableItem);
            ++i;
        }
        m_ui->sectionsTable->setSortingEnabled(true);
        m_ui->sectionsTable->resizeRowsToContents();
        m_ui->sectionsTable->resizeColumnsToContents();

        // program headers table
        i = 0;
        const std::vector<AbstractProgramHeader>& programs(m_parser->getProgramHeaders().getProgramHeaders());
        m_ui->programsTable->setRowCount(programs.size());
        m_ui->programsTable->setSortingEnabled(false);
        BOOST_FOREACH(const AbstractProgramHeader& program, programs)
        {
            tableItem = new QTableWidgetItem(QString(program.getName().c_str()));
            m_ui->programsTable->setItem(i, 0, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(program.getOffset());
            m_ui->programsTable->setItem(i, 1, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(program.getVirtualAddress(), true);
            m_ui->programsTable->setItem(i, 2, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(program.getPhysicalAddress(), true);
            m_ui->programsTable->setItem(i, 3, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(program.getFileSize());
            m_ui->programsTable->setItem(i, 4, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new IntWidgetItem(program.getMemorySize());
            m_ui->programsTable->setItem(i, 5, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new QTableWidgetItem(QString(program.getFlagsString().c_str()));
            m_ui->programsTable->setItem(i, 6, tableItem);
            m_tableItems.push_back(tableItem);
            ++i;
        }
        m_ui->programsTable->setSortingEnabled(true);
        m_ui->programsTable->resizeRowsToContents();
        m_ui->programsTable->resizeColumnsToContents();

        // symbols
        i = 0;
        const std::vector<AbstractSymbol>& allSymbols(m_parser->getSegments().getAllSymbols());
        m_ui->symbolsTable->setRowCount(allSymbols.size());
        m_ui->symbolsTable->setSortingEnabled(false);
        BOOST_FOREACH(const AbstractSymbol& symbol, allSymbols)
        {
            tableItem = new QTableWidgetItem(QString(symbol.getTypeName().c_str()));
            m_ui->symbolsTable->setItem(i, 0, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new QTableWidgetItem(QString(symbol.getBinding().c_str()));
            m_ui->symbolsTable->setItem(i, 1, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new QTableWidgetItem(QString(symbol.getName().c_str()));
            m_ui->symbolsTable->setItem(i, 2, tableItem);
            m_tableItems.push_back(tableItem);
            ++i;
        }
        m_ui->symbolsTable->setSortingEnabled(true);
        m_ui->symbolsTable->resizeColumnsToContents();

        // capabilities tree
        const std::map<elf::Capabilties, std::set<std::string> >& capabilities(m_parser->getCapabilties());
        for (std::map<elf::Capabilties, std::set<std::string> >::const_iterator it = capabilities.begin();
             it != capabilities.end(); ++it)
        {
            QTreeWidgetItem* rootItem = new QTreeWidgetItem(m_ui->capabilitiesTree);
            switch (it->first)
            {
                case elf::k_fileFunctions:
                    rootItem->setText(0, QString("File Functions"));
                    break;
                case elf::k_networkFunctions:
                    rootItem->setText(0, QString("Network Functions"));
                    break;
                case elf::k_processManipulation:
                    rootItem->setText(0, QString("Process Manipulation"));
                    break;
                case elf::k_pipeFunctions:
                    rootItem->setText(0, QString("Pipe Functions"));
                    break;
                case elf::k_crypto:
                    rootItem->setText(0, QString("Random Functions"));
                    break;
                case elf::k_infoGathering:
                    rootItem->setText(0, QString("Information Gathering"));
                    break;
                case elf::k_envVariables:
                    rootItem->setText(0, QString("Environment Variables"));
                    break;
                case elf::k_permissions:
                    rootItem->setText(0, QString("Permissions"));
                    break;
                case elf::k_syslog:
                    rootItem->setText(0, QString("System Log"));
                    break;
                case elf::k_packetSniff:
                    rootItem->setText(0, QString("Packet Sniffing"));
                    break;
                case elf::k_shell:
                    rootItem->setText(0, QString("Shell"));
                    break;
                case elf::k_packed:
                    rootItem->setText(0, QString("Packed"));
                    break;
                case elf::k_irc:
                    rootItem->setText(0, QString("IRC"));
                    break;
                case elf::k_http:
                    rootItem->setText(0, QString("HTTP"));
                    break;
                case elf::k_compression:
                    rootItem->setText(0, QString("Compression"));
                    break;
                case elf::k_ipAddress:
                    rootItem->setText(0, QString("IP Addresses"));
                    break;
                case elf::k_url:
                    rootItem->setText(0, QString("URL"));
                    break;
                case elf::k_hooking:
                    rootItem->setText(0, QString("Function Hooking"));
                    break;
                case elf::k_antidebug:
                    rootItem->setText(0, QString("Anti-Debug"));
                    break;
                case elf::k_dropper:
                    rootItem->setText(0, QString("Dropper"));
                    break;
                default:
                    rootItem->setText(0, QString("Unassigned"));
                    break;
            }
            m_ui->capabilitiesTree->addTopLevelItem(rootItem);

            BOOST_FOREACH(const std::string& child, it->second)
            {
                QTreeWidgetItem* childItem = new QTreeWidgetItem(rootItem);
                childItem->setText(1, QString(child.c_str()));
                m_treeItems.push_back(childItem);
            }
            m_treeItems.push_back(rootItem);
        }
        m_ui->capabilitiesTree->resizeColumnToContents(0);
        m_ui->capabilitiesTree->resizeColumnToContents(1);

        // score listing
        i = 0;
        const std::vector<std::pair<boost::int32_t, std::string> >& reasons(m_parser->getReasons());
        m_ui->scoringTable->setRowCount(reasons.size());
        m_ui->scoringTable->setSortingEnabled(false);
        for (std::vector<std::pair<boost::int32_t, std::string> >::const_iterator reason = reasons.begin();
             reason != reasons.end(); ++reason)
        {
            tableItem = new IntWidgetItem(reason->first);
            m_ui->scoringTable->setItem(i, 0, tableItem);
            m_tableItems.push_back(tableItem);
            tableItem = new QTableWidgetItem(QString(reason->second.c_str()));
            m_ui->scoringTable->setItem(i, 1, tableItem);
            m_tableItems.push_back(tableItem);
            ++i;
        }
        m_ui->scoringTable->setSortingEnabled(true);
        m_ui->scoringTable->resizeColumnsToContents();
    }
}

void MainWindow::overviewToClipboard()
{
    QItemSelectionModel* selected = m_ui->overviewTable->selectionModel();
    if (selected->hasSelection())
    {
        BOOST_FOREACH(const QModelIndex& index, selected->selectedRows())
        {
            QApplication::clipboard()->setText(m_ui->overviewTable->item(index.row(), 0)->text());
        }
    }
}

void MainWindow::sectionSelected(QTableWidgetItem* p_first, QTableWidgetItem* p_second)
{
    QTableWidgetItem* selected = m_ui->sectionsTable->item(p_first->row(), 5);
    std::string details(m_parser->getSegments().printSegment(boost::lexical_cast<boost::uint64_t>(selected->text().toStdString())));
    m_ui->sectionInfo->setPlainText(QString(details.c_str()));
}

void MainWindow::programSelected(QTableWidgetItem* p_first, QTableWidgetItem* p_second)
{
    QTableWidgetItem* selected = m_ui->programsTable->item(p_first->row(), 1);
    std::string details(m_parser->getSegments().printSegment(boost::lexical_cast<boost::uint64_t>(selected->text().toStdString())));
    m_ui->programsInfo->setPlainText(QString(details.c_str()));
}

void MainWindow::reset()
{
    m_tableItems.clear();
    m_treeItems.clear();
    m_parser.reset();
    m_ui->overviewTable->clearContents();
    m_ui->headerTable->clearContents();
    m_ui->sectionsTable->clearContents();
    m_ui->programsTable->clearContents();
    m_ui->scoringTable->clearContents();
    m_ui->capabilitiesTree->clear();
    m_ui->scoreDisplay->display(0);
    m_ui->sectionInfo->clear();
    m_ui->programsInfo->clear();
}

void MainWindow::closeAbout()
{

}

void MainWindow::on_aboutButton_clicked()
{
    if (m_dialog == NULL)
    {
        m_dialog.reset(new QDialog(this));
        Ui_About aboutUi;
        aboutUi.setupUi(m_dialog.get());
        QPixmap image;
        std::string dir;
#ifdef APPLE
        dir.assign(QApplication::applicationDirPath().toStdString());
        boost::replace_last(dir, "MacOS", "Resources/icon_72529.png");
#elif WINDOWS
        dir.assign("./icon_72529.png");
#else
        dir.assign("/usr/local/share/elfparser/icon_72529.png");
#endif
        if (!image.load(QString(dir.c_str())))
        {
            aboutUi.imageLabel->setText("Failed to load image");
        }
        else
        {
            aboutUi.imageLabel->setPixmap(image);
        }
    }
    m_dialog->show();
}

#endif
