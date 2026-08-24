import ROOT

f1 = ROOT.TFile("output_0point5MeV.root")
f2 = ROOT.TFile("output_1MeV0.root")

h1 = f1.Get("Edep")
h2 = f2.Get("Edep")

h1.SetLineColor(ROOT.kBlue)
h2.SetLineColor(ROOT.kRed)

c = ROOT.TCanvas()
h1.Draw()
h2.Draw("SAME")

legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
legend.AddEntry(h1, "Run 0", "l")
legend.AddEntry(h2, "Run 1", "l")
legend.Draw()

c.SaveAs("comparison.png")



