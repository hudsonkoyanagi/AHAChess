#include "ChessController.h"
#include "ChessModel.h"
#include "ChessTextView.h"
#include "Cord.h"
#include "ChessGraphicView.h"

int main() {
    ChessTextView *tv = new ChessTextView{};
    ChessGraphicView *view2 = new ChessGraphicView{1000};
    ChessModel model;
    model.register_view(tv);
    model.register_view(view2);
    ChessController controller{&model};
    controller.input_loop();
    return 0;
}