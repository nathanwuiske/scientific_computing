#include <sstream>
#include "mpi.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class board
{
public:
	vector<char> state_of_previous;
	vector<char> vector_queens;

vector<char> &get_board_state()
{
	return vector_queens;
}

board(int boardSize):vector_queens(boardSize)
{
}

bool gt_next(int start_coord, int finish_coord)
{
	state_of_previous = vector_queens;
	return gt_next(start_coord, finish_coord, start_coord);
}

bool try_to_place(int coord_1, int coord_2)
{
	for(int i=0; i<coord_1; i++)
	{
		int diff = coord_1-i;
		if(vector_queens[i]==coord_2)
		{
			return false;
		}
		else if(vector_queens[i]==(coord_2-diff))
		{
			return false;
		}
		else if(vector_queens[i]==(coord_2+diff))
		{
			return false;
		}
	}
	vector_queens[coord_1]=coord_2;
	return true;
}

bool gt_next(int start_coord, int finish_coord, int coord)
{
	for(int i=vector_queens[coord]; i<vector_queens.size(); i++)
	{
		if(!try_to_place(coord, i))
		{
			continue;
		}
		if((coord+1)<finish_coord)
		{
			if(gt_next(start_coord, finish_coord, coord+1))
			{
				return true;
			}
		}
		else if(vector_queens!=state_of_previous)
		{
			return true;
		}
	}
	vector_queens[coord]=0;
	return false;
}
};

class Messaging
{
public:
	enum enum_set_message_tag
	{
		finished = 2,
		working_response = 1,
		working_request = 0,
	};
	vector<char> board;
	enum_set_message_tag tag;

enum_set_message_tag get_Tag() const
{
	return tag;
}

void message_tag(enum_set_message_tag tag)
{
}
Messaging(int boardSize)
{
	board.resize(boardSize);
}

static bool receive(Messaging* message, int *source)
{
	MPI_Status status;
	MPI_Recv((void*)(&message->board[0]),message->board.size(),MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
	if(source!=0)
	{
		*source = status.MPI_SOURCE;
	}	
	message->message_tag(static_cast<enum_set_message_tag>(status.MPI_TAG));
	return true;
}
	
static bool send(const Messaging *message, int della)
{
	MPI_Send((void*)(&message->board[0]),message->board.size(),MPI_CHAR,della,static_cast<int>(message->get_Tag()),MPI_COMM_WORLD);
	return true;	
}
};


int main(int argc, char **argv)
{
	int n = 10, k = 0;	        	
	int rank, size;	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(n==1) // 1 queen
	{
		goto finalize;
	}
	if(rank==0) //master
	{
		board board(n);
		while(board.gt_next(0, k))
		{
			int proce;
			Messaging request_work_message(n);
			Messaging::receive(&request_work_message, &proce); //get work request
			Messaging work_message(n);
			work_message.message_tag(Messaging::working_response);
			work_message.board = board.get_board_state();
			Messaging::send(&work_message, proce);	// assign task
		}
		for(int i=1; i<size; i++)
		{
			int proce;
			Messaging request_work_message(n);
			Messaging::receive(&request_work_message, &proce);	//get work request
			Messaging kill_message(n); 
			kill_message.message_tag(Messaging::finished);
			Messaging::send(&kill_message, proce);	//kill signal to slaves
		}
	}
	else // slaves
	{
		while(true)
		{
			Messaging request_work_message(n);
			request_work_message.message_tag(Messaging::working_request);
			Messaging::send(&request_work_message, 0); //request work from master
			Messaging work_message(n);
			Messaging::receive(&work_message, 0); //recieve work
			if(Messaging::finished == work_message.get_Tag())
			{
				break;
			}
			board part(n);
			part.get_board_state() = work_message.board;
		}
	}
finalize:
	MPI_Finalize();
}