#include "ChessController.h"
#include "ChessModel.h"
#include "ChessTextView.h"
#include "Cord.h"
// #include "ChessGraphicView.h"

int main() {
    ChessTextView *tv = new ChessTextView{};
    // ChessGraphicView *view = new ChessGraphicView{400};
    // ChessGraphicView *view2 = new ChessGraphicView{};
    ChessModel model;
    model.register_view(tv);
    // model.register_view(view);
    // model.register_view(view2);
    ChessController controller{&model};
    // controller.input_loop();
    controller.setup_loop();
    

    // model.attempt_move(Cord{7,1}, Cord{5,2}, true);

    

    // controller.input_loop();
    return 0;
}