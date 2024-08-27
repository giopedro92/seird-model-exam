void setStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(20);
  gStyle->SetOptTitle(0);
}

void graph() {
  // CANVAS
  TCanvas *c = new TCanvas("c", "SIR", 700, 500);
  TMultiGraph *mg = new TMultiGraph();

  // GRAPH
  Int_t MarkerStyle = 1;

  TGraph *graphS = new TGraph("data.dat", "%lg %lg"); // Suscettibili
  graphS->SetMarkerStyle(MarkerStyle);
  graphS->SetMarkerColor(kBlue);
  graphS->SetLineColor(kBlue);
  mg->Add(graphS);

  TGraph *graphE = new TGraph("data.dat", "%lg %*lg %lg"); // Esposti
  graphE->SetMarkerStyle(MarkerStyle);
  graphE->SetMarkerColor(kOrange+10);
  graphE->SetLineColor(kOrange+10);
  mg->Add(graphE);

  TGraph *graphI = new TGraph("data.dat", "%lg %*lg %*lg %lg"); // Infetti
  graphI->SetMarkerStyle(MarkerStyle);
  graphI->SetMarkerColor(kMagenta);
  graphI->SetLineColor(kMagenta);
  mg->Add(graphI);

  TGraph *graphR = new TGraph("data.dat", "%lg %*lg %*lg %*lg %lg"); // Recovered
  graphR->SetMarkerStyle(MarkerStyle);
  graphR->SetMarkerColor(kGreen);
  graphR->SetLineColor(kGreen);
  mg->Add(graphR);

  TGraph *graphD = new TGraph("data.dat", "%lg %*lg %*lg %*lg %*lg %lg"); // Dead
  graphD->SetMarkerStyle(MarkerStyle);
  graphD->SetMarkerColor(kOrange);
  graphD->SetLineColor(kOrange);
  mg->Add(graphD);

  // LINES
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

  // AXIS
  mg->GetXaxis()->SetTitle("Days");
  mg->GetYaxis()->SetTitle("People");
  mg->Draw("APL");

  // LEGEND
  TLegend *leg = new TLegend(0.9, 0.7, 1, 1, "Legend");
  leg->SetFillColor(0);
  leg->AddEntry(graphS, "Susceptible");
  leg->AddEntry(graphE, "Exposed");
  leg->AddEntry(graphI, "Infectious");
  leg->AddEntry(graphR, "Recovered");
  leg->AddEntry(graphD, "Dead");
  leg->Draw("SAME");
  if (thr != 0) {
    line->Draw("same");
  }
  c->Print("SIR.pdf");
}