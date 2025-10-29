#include "paperengine.h"
#include <algorithm>

PaperEngine::PaperEngine(double initial_cash)
    : cash(initial_cash), pos_qty(0.0), pos_avg_price(0.0) {}

Order PaperEngine::execute(const Order& order) {
    std::lock_guard<std::mutex> lk(mtx);
    Order out = order;
    // Em paper mode, preenchimento imediato se houver caixa (simplificado)
    if(order.side == "BUY") {
        double cost = order.qty * order.price;
        if(cost <= cash) {
            // atualiza avg price
            double new_qty = pos_qty + order.qty;
            if(pos_qty == 0.0) pos_avg_price = order.price;
            else pos_avg_price = (pos_avg_price * pos_qty + order.price * order.qty) / new_qty;
            pos_qty = new_qty;
            cash -= cost;
            out.status = "FILLED";
        } else {
            out.status = "REJECTED";
        }
    } else if(order.side == "SELL") {
        double sell_qty = std::min(order.qty, pos_qty);
        if(sell_qty <= 0.0) {
            out.status = "REJECTED";
        } else {
            double proceeds = sell_qty * order.price;
            pos_qty -= sell_qty;
            if(pos_qty <= 0.0) pos_avg_price = 0.0;
            cash += proceeds;
            out.qty = sell_qty;
            out.status = "FILLED";
        }
    } else {
        out.status = "REJECTED";
    }

    trades.push_back(out);
    return out;
}

double PaperEngine::get_cash() {
    std::lock_guard<std::mutex> lk(mtx);
    return cash;
}

double PaperEngine::get_position_qty() {
    std::lock_guard<std::mutex> lk(mtx);
    return pos_qty;
}

double PaperEngine::get_avg_price() {
    std::lock_guard<std::mutex> lk(mtx);
    return pos_avg_price;
}

const std::vector<Order>& PaperEngine::get_trades() const {
    return trades;
}
