//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}


int lsort(long n, long m, int k)
{
	return false;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListView1ColumnClick(TObject *Sender,
      TListColumn *Column)
{
	//ListView1->CustomSort(lsort);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	TListItem* a = ListView1->Items->Add();
	a->Caption="sdfsdf";
	TStrings * n = a->SubItems;
	n->Add("fsdfdf");
}
//---------------------------------------------------------------------------

