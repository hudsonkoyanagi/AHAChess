#include "ChessController.h"
#include "ChessModel.h"
#include "ChessTextView.h"

int main() {

    ChessTextView view;
    ChessModel model{&view};
    ChessController controller{&model};

    controller.input_loop();
    return 0;
}