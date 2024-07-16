#include "ChessController.h"
#include "ChessModel.h"
#include "ChessTextView.h"
#include "ChessGraphicView.h"

int main() {

    ChessGraphicView view;
    ChessModel model{&view};
    ChessController controller{&model};

    controller.input_loop();
    return 0;
}