void setStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(20);
  gStyle->SetOptTitle(0);
}

void graph() {
  TCanvas *c = new TCanvas("c", "SIR", 700, 500);
  TMultiGraph *mg = new TMultiGraph();

  TGraph *graphS = new TGraph("data.dat", "%lg %lg");
  graphS->SetMarkerStyle(20);
  graphS->SetMarkerColor(kBlue);
  graphS->SetLineColor(kBlue);
  mg->Add(graphS);

  TGraph *graphI = new TGraph("data.dat", "%lg %*lg %lg");
  graphI->SetMarkerStyle(20);
  graphI->SetMarkerColor(kRed);
  graphI->SetLineColor(kRed);
  mg->Add(graphI);

  TGraph *graphR = new TGraph("data.dat", "%lg %*lg %*lg %lg");
  graphR->SetMarkerStyle(20);
  graphR->SetMarkerColor(kGreen);
  graphR->SetLineColor(kGreen);
  mg->Add(graphR);

  std::ifstream input_file("threshold.dat");
  double thr;
  input_file >> thr;
  std::cout << thr << '\n';

  std::ifstream input_file1("population.dat");
  int pop;
  input_file1 >> pop;
  std::cout << pop;

  TLine *line = new TLine(thr, 0, thr, pop);
  line->SetLineWidth(2);
  // line->SetLineColor();
  line->SetLineStyle(kDashed);

  mg->GetXaxis()->SetTitle("Days");
  mg->GetYaxis()->SetTitle("People");
  mg->Draw("APL");

  TLegend *leg = new TLegend(0.9, 0.7, 1, 1, "Legend");
  leg->SetFillColor(0);
  leg->AddEntry(graphS, "Susceptible");
  leg->AddEntry(graphI, "Infectious");
  leg->AddEntry(graphR, "Recovered");
  leg->Draw("SAME");
  if (thr != 0) {
    line->Draw("same");
  }
  c->Print("SIR.pdf");
}