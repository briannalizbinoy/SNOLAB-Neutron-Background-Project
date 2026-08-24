import ROOT

# Open input ROOT files
f1 = ROOT.TFile("output_0p5MeV_neutrons_He40.root")
f2 = ROOT.TFile("output_1MeV_neutrons_He40.root")
f3 = ROOT.TFile("output_2MeV_neutrons_He40.root")
f4 = ROOT.TFile("output_3MeV_neutrons_He40.root")

# Get histograms
h1 = f1.Get("Edep")
h2 = f2.Get("Edep")
h3 = f3.Get("Edep")
h4 = f4.Get("Edep")

# Check that histograms loaded properly
for i, h in enumerate([h1, h2, h3, h4], start=1):
    if not h:
        print(f"⚠️ Histogram Edep not found in file f{i}")
        exit(1)

# Style settings
colors = [ROOT.kRed, ROOT.kBlue, ROOT.kGreen + 2, ROOT.kMagenta]
labels = ["0.5 MeV", "1 MeV", "2 MeV", "3 MeV"]

for h, color in zip([h1, h2, h3, h4], colors):
    h.SetLineColor(color)
    h.SetLineWidth(2)

# Create a canvas and draw
c = ROOT.TCanvas("c", "Energy Deposition Comparison", 800, 600)

# Draw first histogram normally
h1.Draw("HIST")

# Draw the rest on top
for h in [h2, h3, h4]:
    h.Draw("SAME HIST")

# Add legend
legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
for h, label in zip([h1, h2, h3, h4], labels):
    legend.AddEntry(h, f"{label} run", "l")
legend.Draw()
c.SetLogy()  # optional: log scale on Y-axis
c.SetGrid()  # optional: show grid

# --- Save as ROOT file ---
outFile = ROOT.TFile("comparison.root", "RECREATE")
c.Write("ComparisonCanvas")

for h, label in zip([h1, h2, h3, h4], labels):
    h.Write(f"Edep_{label.replace(' ', '')}")

outFile.Close()

print("✅ Saved as comparison.root")

