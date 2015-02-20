#include "CLI.h"


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <Commnd line user interface>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void CLI::showConsoleHeader() {		
	cout << "####################################################################################################\n";
	cout << "  >>>                                    Traffic sign detection                                <<<  \n";
	cout << "####################################################################################################\n\n";
}


void CLI::startInteractiveCLI() {
	int userOption = 0;
	string filename = "";
	int cameraDeviceNumber = 0;
	
	ConsoleInput::getInstance()->clearConsoleScreen();
	showConsoleHeader();
	
	int screenWidth = ConsoleInput::getInstance()->getIntCin("  >> Screen width (used to arrange windows): ", "  => Width >= 100 !!!\n", 100);
	int screenHeight = ConsoleInput::getInstance()->getIntCin("  >> Screen height (used to arrange windows): ", "  => Width >= 100 !!!\n", 100);
	bool optionsOneWindow = ConsoleInput::getInstance()->getYesNoCin("  >> Use only one window for options trackbars? (Y/N): ");

	do {
		ConsoleInput::getInstance()->clearConsoleScreen();
		showConsoleHeader();
		
		userOption = getUserOption();

		if (userOption > 0 && userOption < 3) {
			filename = "";
			do {
				cout << "  >> Path to file: ";
				filename = ConsoleInput::getInstance()->getLineCin();

				if (filename == "") {
					cerr << "  => File path can't be empty!\n" << endl;
				}
			} while (filename == "");
		}

		if (userOption == 3) {
			cameraDeviceNumber = ConsoleInput::getInstance()->getIntCin("  >> Insert the camera device number to use (default: 0): ", "  => Camera device number must be >= 0 !!!\n", 0);
		}

		try {
			ImageAnalysis imageAnalysis;
			imageAnalysis.ScreenWidth(screenWidth);
			imageAnalysis.ScreenHeight(screenHeight);
			imageAnalysis.OptionsOneWindow(optionsOneWindow);
			switch (userOption) {
				case 1: {
					if (!imageAnalysis.processImage(filename)) {
						cerr << "  => Failed to load image " << filename << "!" << endl;
					}
					break;
				}

				case 2: {
					if (!imageAnalysis.processVideo(filename)) {
						cerr << "  => Failed to load video " << filename << "!" << endl;
					}
					break;
				}

				case 3: {				
					if (!imageAnalysis.processVideo(cameraDeviceNumber)) {
						cerr << "  => Failed to open camera " << cameraDeviceNumber << "!" << endl;
					}
					break;
				}

				default:
					break;
			}
		} catch(...) {}

		if (userOption != 0) {
			cout << "\n\n" << endl;
			ConsoleInput::getInstance()->getUserInput();
		}
	} while (userOption != 0);

	cout << "\n\n\n" << endl;
	showVersion();
	cout << "\n\n" << endl;
	ConsoleInput::getInstance()->getUserInput();
}


int CLI::getUserOption() {
	cout << " ## Detect traffic sign from:\n";
	cout << "   1 - Image\n";
	cout << "   2 - Video\n";
	cout << "   3 - Camera\n";
	cout << "   0 - Exit\n";

	return ConsoleInput::getInstance()->getIntCin("\n >>> Option [0, 3]: ", "Select one of the options above!", 0, 4);	
}


void CLI::showVersion() {
	cout << "+==================================================================================================+" << endl;
	cout << "+==================================================================================================+" << endl;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </Commnd line user interface>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
