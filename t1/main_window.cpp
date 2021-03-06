#include <iostream>
#include <string>
#include "main_window.h"

MainWindow::MainWindow() {
	gomoku_core = GomokuCore::instance();
	load_grid();
	load_box_layout();

	set_border_width(16);
	add(*v_box);
	// set_resizable(false);
	mode = H_VS_H;
}

MainWindow::~MainWindow() {
	delete btn_restart;
	// delete btn_human_vs_human;
	// delete btn_human_vs_computer;
	// delete btn_computer_vs_computer;
	for (auto i = 0; i < GRID_SIZE; ++i) {
		for (auto j = 0; j < GRID_SIZE; ++j) {
			delete btn_grid[i][j];
		}
	}
	delete grid;
	delete h_box;
	delete v_box;

}

void MainWindow::on_grid_button_clicked(int x, int y,
		Gtk::Button* clicked_button) {
	std::string label;
	if (gomoku_core->game_state() == END_GAME) {
		return;
	}

	if (mode != IA_VS_IA) {
		if (gomoku_core->compute_play(x, y)) {
			if (gomoku_core->game_over()) {
				gomoku_core->game_state(END_GAME);
				lbl_info->set_label(
						"Player "
								+ std::to_string(gomoku_core->player_turn() + 1)
								+ " wins");
				label = (gomoku_core->player_turn() == P1) ? "O" : "X";
				clicked_button->set_label(label);
				return;
			}
			label = (gomoku_core->player_turn() == P1) ? "O" : "X";
			clicked_button->set_label(label);

			gomoku_core->change_turn();
			lbl_info->set_label(
					"Player " + std::to_string(gomoku_core->player_turn() + 1)
							+ " turn");

			if (mode == H_VS_IA) {
				std::tuple<int, int> play = gomoku_core->minimax(4);
				lbl_iteracoes->set_label(
						"Iteracoes: "
								+ std::to_string(
										gomoku_core->conta_iteracoes()));
				if (gomoku_core->compute_play(std::get < 0 > (play),
						std::get < 1 > (play))) {
					if (gomoku_core->game_over()) {
						gomoku_core->game_state(END_GAME);
						lbl_info->set_label(
								"Player "
										+ std::to_string(
												gomoku_core->player_turn() + 1)
										+ " wins");
						label = (gomoku_core->player_turn() == P1) ? "O" : "X";
						btn_grid[std::get < 0 > (play)][std::get < 1 > (play)]->set_label(
								label);
					} else {
						label = (gomoku_core->player_turn() == P1) ? "O" : "X";
						btn_grid[std::get < 0 > (play)][std::get < 1 > (play)]->set_label(
								label);
						gomoku_core->change_turn();
						lbl_info->set_label(
								"Player "
										+ std::to_string(
												gomoku_core->player_turn() + 1)
										+ " turn");
					}
				}
			}
		}

	}

}

void MainWindow::on_restart_button_clicked(Gtk::Button* clicked_button) {
	gomoku_core->restart();
	lbl_info->set_label(
			"Player " + std::to_string(gomoku_core->player_turn() + 1)
					+ " turn");

	std::string label;
	for (auto i = 0; i < GRID_SIZE; ++i) {
		for (auto j = 0; j < GRID_SIZE; ++j) {
			//label = std::to_string(i) + "," + std::to_string(j);
			label = "    ";
			btn_grid[i][j]->set_label(label);
		}
	}

}

void MainWindow::on_human_vs_human_button_clicked(Gtk::Button* clicked_button) {
	if (gomoku_core->game_state() == NEW_GAME)
		mode = H_VS_H;
}

void MainWindow::on_human_vs_computer_button_clicked(
		Gtk::Button* clicked_button) {
	if (gomoku_core->game_state() == NEW_GAME)
		mode = H_VS_IA;
}

void MainWindow::on_next_play(Gtk::Button* clicked_button) {
	if(mode == IA_VS_IA)
		ia_vs_ia();
}

void MainWindow::ia_vs_ia() {
	std::string label;
	std::tuple<int, int> play = gomoku_core->minimax(4);
	lbl_iteracoes->set_label(
			"Iteracoes: " + std::to_string(gomoku_core->conta_iteracoes()));
	if (gomoku_core->compute_play(std::get < 0 > (play),
			std::get < 1 > (play))) {
		if (gomoku_core->game_over()) {
			gomoku_core->game_state(END_GAME);
			lbl_info->set_label(
					"Player " + std::to_string(gomoku_core->player_turn() + 1)
							+ " wins");
			label = (gomoku_core->player_turn() == P1) ? "O" : "X";
			btn_grid[std::get < 0 > (play)][std::get < 1 > (play)]->set_label(
					label);
		} else {
			label = (gomoku_core->player_turn() == P1) ? "O" : "X";
			btn_grid[std::get < 0 > (play)][std::get < 1 > (play)]->set_label(
					label);
			gomoku_core->change_turn();
			lbl_info->set_label(
					"Player " + std::to_string(gomoku_core->player_turn() + 1)
							+ " turn");

		}
	}
}


void MainWindow::on_computer_vs_computer_button_clicked(
	Gtk::Button* clicked_button) {

if (gomoku_core->game_state() == NEW_GAME) {
	mode = IA_VS_IA;
	//ia_thread = new std::thread(&MainWindow::ia_vs_ia, this);
	//ia_vs_ia_thread = Glib::Thread::create(
	//		sigc::mem_fun(*this, &MainWindow::ia_vs_ia), true);
}

}

void MainWindow::on_computer_vs_human_button_clicked(
	Gtk::Button* clicked_button) {
std::string label;

if (gomoku_core->game_state() == NEW_GAME) {
	mode = H_VS_IA;

	gomoku_core->change_turn();
	std::tuple<int, int> play = gomoku_core->minimax(4);
	lbl_iteracoes->set_label(
			"Iteracoes: "
					+ std::to_string(
							gomoku_core->conta_iteracoes()));
	if (gomoku_core->compute_play(std::get < 0 > (play),
			std::get < 1 > (play))) {
		if (gomoku_core->game_over()) {
			gomoku_core->game_state(END_GAME);
			lbl_info->set_label(
					"Player " + std::to_string(gomoku_core->player_turn() + 1)
							+ " wins");
			label = (gomoku_core->player_turn() == P1) ? "O" : "X";
			btn_grid[std::get < 0 > (play)][std::get < 1 > (play)]->set_label(
					label);
		} else {
			label = (gomoku_core->player_turn() == P1) ? "O" : "X";
			btn_grid[std::get < 0 > (play)][std::get < 1 > (play)]->set_label(
					label);
			gomoku_core->change_turn();
			lbl_info->set_label(
					"Player " + std::to_string(gomoku_core->player_turn() + 1)
							+ " turn");
		}
	}
}
}

void MainWindow::load_grid() {
grid = Gtk::manage(new Gtk::Grid());

std::string label;
for (auto i = 0; i < GRID_SIZE; ++i) {
	if (i) {
		label = "    "; //std::to_string(i) + "," + "0";
		btn_grid[i][0] = Gtk::manage(new Gtk::Button(label));
		btn_grid[i][0]->signal_clicked().connect(
				sigc::bind<int>(
						sigc::bind<int>(
								sigc::bind<Gtk::Button*>(
										sigc::mem_fun(*this,
												&MainWindow::on_grid_button_clicked),
										btn_grid[i][0]), 0), i));
		btn_grid[i][0]->set_hexpand(true);
		btn_grid[i][0]->set_vexpand(true);

		grid->attach_next_to(*btn_grid[i][0], *btn_grid[i - 1][0],
				Gtk::POS_BOTTOM, 1, 1);
		btn_grid[i][0]->show();
	} else {
		label = "    "; //std::to_string(i) + "," + "0";
		btn_grid[i][0] = Gtk::manage(new Gtk::Button(label));
		btn_grid[i][0]->signal_clicked().connect(
				sigc::bind<int>(
						sigc::bind<int>(
								sigc::bind<Gtk::Button*>(
										sigc::mem_fun(*this,
												&MainWindow::on_grid_button_clicked),
										btn_grid[i][0]), 0), i));
		btn_grid[i][0]->set_hexpand(true);
		btn_grid[i][0]->set_vexpand(true);
		grid->attach_next_to(*btn_grid[i][0], Gtk::POS_RIGHT, 1, 1);
		btn_grid[i][0]->show();
	}
	for (auto j = 0; j < GRID_SIZE; ++j) {
		if (j) {
			label = "    "; //std::to_string(i) + "," + std::to_string(j);
			btn_grid[i][j] = Gtk::manage(new Gtk::Button(label));
			btn_grid[i][j]->signal_clicked().connect(
					sigc::bind<int>(
							sigc::bind<int>(
									sigc::bind<Gtk::Button*>(
											sigc::mem_fun(*this,
													&MainWindow::on_grid_button_clicked),
											btn_grid[i][j]), j), i));
			btn_grid[i][j]->set_hexpand(true);
			btn_grid[i][j]->set_vexpand(true);
			grid->attach_next_to(*btn_grid[i][j], *btn_grid[i][j - 1],
					Gtk::POS_RIGHT, 1, 1);
			btn_grid[i][j]->show();
		}
	}
}

grid->show();
}

void MainWindow::load_box_layout() {
v_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 8));
v_box->pack_start(*grid, true, true, 0);

h_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 4));

load_buttons();

h_box->show();

v_box->pack_start(*h_box);
v_box->show();
}

void MainWindow::load_buttons() {
btn_restart = Gtk::manage(new Gtk::Button("Restart"));
btn_restart->signal_clicked().connect(
		sigc::bind<Gtk::Button*>(
				sigc::mem_fun(*this, &MainWindow::on_restart_button_clicked),
				btn_restart));
// btn_restart -> override_color(Gdk::RGBA("red"));
btn_restart->show();

btn_human_vs_human = Gtk::manage(new Gtk::Button("vs Human"));
btn_human_vs_human->signal_clicked().connect(
		sigc::bind<Gtk::Button*>(
				sigc::mem_fun(*this,
						&MainWindow::on_human_vs_human_button_clicked),
				btn_human_vs_human));

btn_human_vs_human->show();

btn_human_vs_computer = Gtk::manage(new Gtk::Button("vs IA"));
btn_human_vs_computer->signal_clicked().connect(
		sigc::bind<Gtk::Button*>(
				sigc::mem_fun(*this,
						&MainWindow::on_human_vs_computer_button_clicked),
				btn_human_vs_computer));

btn_human_vs_computer->show();

btn_computer_vs_computer = Gtk::manage(new Gtk::Button("IA vs IA"));
btn_computer_vs_computer->signal_clicked().connect(
		sigc::bind<Gtk::Button*>(
				sigc::mem_fun(*this,
						&MainWindow::on_computer_vs_computer_button_clicked),
				btn_computer_vs_computer));

btn_computer_vs_computer->show();

btn_computer_vs_human = Gtk::manage(new Gtk::Button("IA vs"));
btn_computer_vs_human->signal_clicked().connect(
		sigc::bind<Gtk::Button*>(
				sigc::mem_fun(*this,
						&MainWindow::on_computer_vs_human_button_clicked),
				btn_computer_vs_human));

btn_computer_vs_human->show();

btn_next_ia_play = Gtk::manage(new Gtk::Button("Next Play"));
btn_next_ia_play->signal_clicked().connect(
		sigc::bind<Gtk::Button*>(
				sigc::mem_fun(*this,
						&MainWindow::on_next_play),
				btn_next_ia_play));

btn_next_ia_play->show();

h_box->pack_start(*btn_restart);
h_box->pack_start(*btn_human_vs_human);
h_box->pack_start(*btn_human_vs_computer);
h_box->pack_start(*btn_computer_vs_computer);
h_box->pack_start(*btn_computer_vs_human);
h_box->pack_start(*btn_next_ia_play);

lbl_info = Gtk::manage(
		new Gtk::Label(
				"Player " + std::to_string(gomoku_core->player_turn() + 1)
						+ " turn"));
lbl_info->show();
h_box->pack_start(*lbl_info);

lbl_iteracoes = Gtk::manage(new Gtk::Label("Iteracoes: 0"));
lbl_iteracoes->show();
h_box->pack_start(*lbl_iteracoes);
}

