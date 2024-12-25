#include "solveproblemwindow.h"
#include "ui_solveproblemwindow.h"
#include "database.h"
#include <qmessagebox.h>
#include "sessionmanager.h"
#include <Qsci/qsciapis.h>
#include "submissionmanager.h"

solveproblemwindow::solveproblemwindow(Database &db, int problemId, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::solveproblemwindow),
    problemId(problemId),
    db(db), 
    process(new QProcess(this)) ,// Add this line to initialize db
    scintilla(new QsciScintilla(this))
{
    ui->setupUi(this);
    this->setStyleSheet(
        "QDialog {"
        "    background-color: #2b2b2b;"
        "    color: white;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #9a76b1;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #6a4a75;"
        "}");
    // Set up the QsciScintilla instance
    QsciLexerCPP *cppLexer = new QsciLexerCPP(this);
    QFont editorFont("Fira Code", 12); // Use a modern font with ligatures
    scintilla->setFont(editorFont);
    cppLexer->setFont(editorFont);
    cppLexer->setDefaultFont(editorFont);

    // Syntax highlighting
    cppLexer->setColor(QColor("#66d9ef"), QsciLexerCPP::Keyword); // Blue for keywords
    cppLexer->setColor(QColor("#a6e22e"), QsciLexerCPP::DoubleQuotedString); // Green for strings
    cppLexer->setColor(QColor("#75715e"), QsciLexerCPP::Comment); // Grey for comments
    cppLexer->setColor(QColor("#f92672"), QsciLexerCPP::Number); // Pink for numbers
    cppLexer->setPaper(QColor("#272822")); // Dark background
    cppLexer->setColor(QColor("#f8f8f2")); // Light text

    scintilla->setLexer(cppLexer);

    // Enable syntax-related features
    scintilla->setAutoIndent(true);
    scintilla->setIndentationGuides(true);
    scintilla->setIndentationWidth(4);
    scintilla->setBraceMatching(QsciScintilla::SloppyBraceMatch);

    // Highlight the current line
    scintilla->setCaretLineVisible(true);
    scintilla->setCaretLineBackgroundColor(QColor("#3e3d32")); // Light grey

    // Line number margin
    scintilla->setMarginType(0, QsciScintilla::NumberMargin);
    scintilla->setMarginsBackgroundColor(QColor("#1e1e1e")); // Dark grey
    scintilla->setMarginsForegroundColor(QColor("#75715e")); // Subtle grey
    scintilla->setMarginWidth(0, QString::number(scintilla->lines()).length() * 10 + 20);

    // Auto-completion
    scintilla->setAutoCompletionSource(QsciScintilla::AcsAll);
    scintilla->setAutoCompletionThreshold(2);
    scintilla->setAutoCompletionCaseSensitivity(false);

    // Set APIs for auto-completion
    QsciAPIs *apis = new QsciAPIs(cppLexer);
    apis->add("int");
    apis->add("float");
    apis->add("double");
    apis->add("return");
    apis->add("<iostream>");
    apis->add("<vector>");
    apis->add("<string>");
    apis->prepare();
    cppLexer->setAPIs(apis);

    // Add QsciScintilla widget to layout
    int row, col, rowSpan, colSpan;
    ui->gridLayout->getItemPosition(ui->gridLayout->indexOf(ui->codeEditor), &row, &col, &rowSpan, &colSpan);
    ui->gridLayout->removeWidget(ui->codeEditor);
    delete ui->codeEditor;
    ui->codeEditor = nullptr;
    ui->gridLayout->addWidget(scintilla, row, col, rowSpan, colSpan);

    // Load problem details from the database
    QSqlQuery query(db.getDatabase());
    query.prepare("SELECT Title, Description, Input_Format, Output_Format, Constraints, Example FROM Problems WHERE ID = ?");
    query.addBindValue(problemId);

    if (query.exec() && query.next()) {
        ui->titleLabel->setText(query.value("Title").toString());
        ui->descriptionText->setPlainText(query.value("Description").toString());
        ui->inputFormatText->setPlainText(query.value("Input_Format").toString());
        ui->outputFormatText->setPlainText(query.value("Output_Format").toString());
        ui->constraintsText->setPlainText(query.value("Constraints").toString());
        ui->exampleText->setPlainText(query.value("Example").toString());
    } else {
        QMessageBox::warning(this, "Error", "Could not load problem details: " + query.lastError().text());
    }

    // Signal-slot for the Run button
    connect(ui->runButton, &QPushButton::clicked, this, &solveproblemwindow::runCode);

    // Dynamic margin adjustment for line numbers
    connect(scintilla, &QsciScintilla::linesChanged, this, [this]() {
        int lineCount = scintilla->lines();
        scintilla->setMarginWidth(0, QString::number(lineCount).length() * 10 + 20);
    });
}



solveproblemwindow::~solveproblemwindow()
{
    delete ui;
}

/*void solveproblemwindow::runCode()
{
    QString userCode = ui->codeEditor->toPlainText();
    QString input = ui->inputText->toPlainText();
    QString outputPath = "user_output.txt";

    // Save user code to a file
    QFile codeFile("user_code.cpp");
    if (!codeFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to save code to file.");
        return;
    }
    QTextStream out(&codeFile);
    out << userCode;
    codeFile.close();

    // Compile the code
    process->start("g++", QStringList() << "-o" << "user_code" << "user_code.cpp");
    process->waitForFinished();

    if (process->exitCode() != 0) // Compilation failed
    {
        QString error = process->readAllStandardError();
        ui->plainTextEdit_4->setPlainText("Compilation Error:\n" + error);
        return;
    }

    // Run the compiled executable
    process->start("./user_code");
    if (!process->waitForStarted())
    {
        ui->plainTextEdit_4->setPlainText("Failed to start the program.");
        return;
    }

    process->waitForFinished();

    if (process->exitCode() == 0) // Execution succeeded
    {
        QString userOutput = process->readAllStandardOutput();
        ui->plainTextEdit_4->setPlainText("Program Output:\n" + userOutput);
    }
    else
    {
        QString error = process->readAllStandardError();
        ui->plainTextEdit_4->setPlainText("Runtime Error:\n" + error);
    }
}

*/
/*void solveproblemwindow::runCode()
{
    QString userCode = scintilla->text();  // Use the member variable

    QString input = ui->inputText->toPlainText();
    QString outputPath = "user_output.txt";

    // Save user code to a file
    QFile codeFile("user_code.cpp");
    if (!codeFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to save code to file.");
        return;
    }
    QTextStream out(&codeFile);
    out << userCode;
    codeFile.close();

    // Compile the code
    process->start("g++", QStringList() << "-o" << "user_code" << "user_code.cpp");
    process->waitForFinished();

    if (process->exitCode() != 0) // Compilation failed
    {
        QString error = process->readAllStandardError();
        ui->plainTextEdit_4->setPlainText("Compilation Error:\n" + error);
        return;
    }

    // Run the compiled executable
    process->start("./user_code");
    if (!process->waitForStarted())
    {
        ui->plainTextEdit_4->setPlainText("Failed to start the program.");
        return;
    }

    // Write input to the program
    process->write(input.toUtf8());
    process->closeWriteChannel(); // Signal end of input

    process->waitForFinished();

    if (process->exitCode() == 0) // Execution succeeded
    {
        QString userOutput = process->readAllStandardOutput();
        ui->plainTextEdit_4->setPlainText("Program Output:\n" + userOutput);
    }
    else
    {
        QString error = process->readAllStandardError();
        ui->plainTextEdit_4->setPlainText("Runtime Error:\n" + error);
    }
}
*/
/*void solveproblemwindow::on_submitButton_clicked() {
    if (!sessionManager::instance().isLoggedIn()) {
        QMessageBox::warning(this, "Error", "No user is logged in. Please log in to submit.");
        return;
    }

    int userId = sessionManager::instance().getUserId();
    QString userCode = scintilla->text();
    int submissionId = db.saveSubmission(userId, problemId, userCode);
    if (submissionId > 0) {
        QMessageBox::information(this, "Success", "Submission saved!");

        // Call saveResults after submission is saved
        saveResults(submissionId);
    } else {
        QMessageBox::warning(this, "Error", "Failed to save submission.");
    }

}*/
void solveproblemwindow::on_submitButton_clicked()
{
    if (!sessionManager::instance().isLoggedIn())
    {
        QMessageBox::warning(this, "Error", "No user is logged in. Please log in to submit.");
        return;
    }

    int userId = sessionManager::instance().getUserId();
    QString userCode = scintilla->text();

    // Create a SubmissionManager instance
    SubmissionManager submissionManager(db);

    // Save the submission and retrieve the submission ID
    int submissionId = submissionManager.saveSubmission(userId, problemId, userCode);

    if (submissionId > 0) {
        QMessageBox::information(this, "Success", "Submission saved!");

        // Call saveResults after the submission is saved
        saveResults(submissionId);
    } else {
        QMessageBox::warning(this, "Error", "Failed to save submission.");
    }
}

/*void solveproblemwindow::runCode()
{
    QString userCode = scintilla->text();

    // Save user code to a file
    QFile codeFile("user_code.cpp");
    if (!codeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to save code to file.");
        return;
    }
    QTextStream out(&codeFile);
    out << userCode;
    codeFile.close();

    // Compile the code
    process->start("g++", QStringList() << "-o" << "user_code" << "user_code.cpp");
    process->waitForFinished();

    if (process->exitCode() != 0) {
        ui->plainTextEdit_4->setPlainText("Compilation Error:\n" + process->readAllStandardError());
        return;
    }

    // Fetch test cases for the problem
    QSqlQuery testCaseQuery = db.getTestCases(problemId);

    int totalCases = 0;
    int passedCases = 0;
    double score = 0.0;
    QString status = "Pending";
    double totalRuntime = 0.0;
    double peakMemoryUsage = 0.0;

    while (testCaseQuery.next()) {
        QString testCaseInput = testCaseQuery.value("Input").toString();
        QString expectedOutput = testCaseQuery.value("ExpectedOutput").toString().trimmed();
        double weight = testCaseQuery.value("Weight").toDouble();

        QElapsedTimer timer;
        timer.start();

        // Run the compiled executable with the test case input
        QProcess testProcess;
        testProcess.setProgram("./user_code");
        testProcess.start();

        if (!testProcess.waitForStarted()) {
            ui->plainTextEdit_4->setPlainText("Failed to start the program.");
            return;
        }

        testProcess.write(testCaseInput.toUtf8());
        testProcess.closeWriteChannel();

        if (!testProcess.waitForFinished()) {
            ui->plainTextEdit_4->setPlainText("Program execution timed out.");
            return;
        }

        // Measure runtime
        double runtime = timer.elapsed() / 1000.0; // Time in seconds
        totalRuntime += runtime;

        // Measure memory usage (placeholder, platform-specific memory profiling tools can replace this)
        // Assuming testProcess has a method for peak memory (if not, use OS-specific tools)
        peakMemoryUsage = std::max(peakMemoryUsage, static_cast<double>(testProcess.processId()));

        QString userOutput = testProcess.readAllStandardOutput().trimmed();

        // Compare output
        if (userOutput == expectedOutput) {
            passedCases++;
            score += weight;
        }

        totalCases++;
    }

    // Determine status
    status = (passedCases == totalCases) ? "Accepted" : "Partial";

    // Display results in the UI
    QString resultSummary = QString("Total Cases: %1\nPassed Cases: %2\nScore: %3\nRuntime: %4 sec\nMemory Used: %5 KB")
                                .arg(totalCases)
                                .arg(passedCases)
                                .arg(score)
                                .arg(totalRuntime)
                                .arg(peakMemoryUsage);

    ui->plainTextEdit_4->setPlainText(resultSummary);

}*/
void solveproblemwindow::runCode()
{
    QString userCode = scintilla->text();

    // Save user code to a file
    QFile codeFile("user_code.cpp");
    if (!codeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to save code to file.");
        return;
    }
    QTextStream out(&codeFile);
    out << userCode;
    codeFile.close();

    // Compile the code
    process->start("g++", QStringList() << "-o" << "user_code" << "user_code.cpp");
    process->waitForFinished();

    if (process->exitCode() != 0) {
        ui->plainTextEdit_4->setPlainText("Compilation Error:\n" + process->readAllStandardError());
        return;
    }

    if (ui->customInputCheckBox->isChecked()) {
        // If custom input checkbox is checked
        QString customInput = ui->customInputTextEdit->toPlainText(); // Assuming a QPlainTextEdit for custom input
        QElapsedTimer timer;
        timer.start();

        // Run the compiled executable with custom input
        QProcess testProcess;
        testProcess.setProgram("./user_code");
        testProcess.start();

        if (!testProcess.waitForStarted()) {
            ui->plainTextEdit_4->setPlainText("Failed to start the program.");
            return;
        }

        testProcess.write(customInput.toUtf8());
        testProcess.closeWriteChannel();

        if (!testProcess.waitForFinished()) {
            ui->plainTextEdit_4->setPlainText("Program execution timed out.");
            return;
        }

        // Measure runtime
        double runtime = timer.elapsed() / 1000.0; // Time in seconds

        QString userOutput = testProcess.readAllStandardOutput().trimmed();
        QString resultSummary = QString("Output:\n%1\n\nRuntime: %2 sec").arg(userOutput).arg(runtime);
        ui->plainTextEdit_4->setPlainText(resultSummary);

    } else {
        // Fetch test cases for the problem
        QSqlQuery testCaseQuery = db.getTestCases(problemId);

        int totalCases = 0;
        int passedCases = 0;
        double score = 0.0;
        QString status = "Pending";
        double totalRuntime = 0.0;
        double peakMemoryUsage = 0.0;

        while (testCaseQuery.next()) {
            QString testCaseInput = testCaseQuery.value("Input").toString();
            QString expectedOutput = testCaseQuery.value("ExpectedOutput").toString().trimmed();
            double weight = testCaseQuery.value("Weight").toDouble();

            QElapsedTimer timer;
            timer.start();

            // Run the compiled executable with the test case input
            QProcess testProcess;
            testProcess.setProgram("./user_code");
            testProcess.start();

            if (!testProcess.waitForStarted()) {
                ui->plainTextEdit_4->setPlainText("Failed to start the program.");
                return;
            }

            testProcess.write(testCaseInput.toUtf8());
            testProcess.closeWriteChannel();

            if (!testProcess.waitForFinished()) {
                ui->plainTextEdit_4->setPlainText("Program execution timed out.");
                return;
            }

            // Measure runtime
            double runtime = timer.elapsed() / 1000.0; // Time in seconds
            totalRuntime += runtime;

            // Measure memory usage (placeholder, platform-specific memory profiling tools can replace this)
            peakMemoryUsage = std::max(peakMemoryUsage, static_cast<double>(testProcess.processId()));

            QString userOutput = testProcess.readAllStandardOutput().trimmed();

            // Compare output
            if (userOutput == expectedOutput) {
                passedCases++;
                score += weight;
            }

            totalCases++;
        }

        // Determine status
        status = (passedCases == totalCases) ? "Accepted" : "Partial";

        // Display results in the UI
        QString resultSummary = QString("Total Cases: %1\nPassed Cases: %2\nScore: %3\nRuntime: %4 sec\nMemory Used: %5 KB")
                                    .arg(totalCases)
                                    .arg(passedCases)
                                    .arg(score)
                                    .arg(totalRuntime)
                                    .arg(peakMemoryUsage);

        ui->plainTextEdit_4->setPlainText(resultSummary);
    }
}

void solveproblemwindow::saveResults(int submissionId)
{
    QString userCode = scintilla->text();

    // Save user code to a file
    QFile codeFile("user_code.cpp");
    if (!codeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to save code to file.");
        return;
    }
    QTextStream out(&codeFile);
    out << userCode;
    codeFile.close();

    // Compile the code
    process->start("g++", QStringList() << "-o" << "user_code" << "user_code.cpp");
    process->waitForFinished();

    if (process->exitCode() != 0)
    {
        QMessageBox::warning(this, "Error", "Compilation failed:\n" + process->readAllStandardError());
        return;
    }

    // Fetch test cases from the database
    QSqlQuery testCaseQuery = db.getTestCases(problemId);
    int totalCases = 0;
    int passedCases = 0;
    double score = 0.0;
    QString status = "Pending";
    double totalRuntime = 0.0;
    double peakMemoryUsage = 0.0;

    while (testCaseQuery.next()) {
        QString testCaseInput = testCaseQuery.value("Input").toString();
        QString expectedOutput = testCaseQuery.value("ExpectedOutput").toString().trimmed();
        double weight = testCaseQuery.value("Weight").toDouble();

        QElapsedTimer timer;
        timer.start();

        QProcess testProcess;
        testProcess.setProgram("./user_code");
        testProcess.start();

        if (!testProcess.waitForStarted()) {
            QMessageBox::warning(this, "Error", "Failed to start the program.");
            return;
        }

        testProcess.write(testCaseInput.toUtf8());
        testProcess.closeWriteChannel();

        if (!testProcess.waitForFinished()) {
            QMessageBox::warning(this, "Error", "Program execution timed out.");
            return;
        }

        double runtime = timer.elapsed() / 1000.0; // Time in seconds
        totalRuntime += runtime;

        QString userOutput = testProcess.readAllStandardOutput().trimmed();
        if (userOutput == expectedOutput) {
            passedCases++;
            score += weight;
        }

        totalCases++;
    }

    status = (passedCases == totalCases) ? "Accepted" : "Partial";
    qDebug() << "Problem ID:" << problemId;
    qDebug() << "Submission ID:" << submissionId;
    qDebug() << "Status:" << status;
    qDebug() << "Runtime:" << totalRuntime;
    qDebug() << "Memory Usage:" << peakMemoryUsage;
    qDebug() << "Score:" << score;
    // Save the results in the database
    if (db.saveResult(submissionId, status, totalRuntime, peakMemoryUsage, score)) {
        QMessageBox::information(this, "Success", "Results saved successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save results to the database.");
    }
}

