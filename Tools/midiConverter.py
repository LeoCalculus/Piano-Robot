import mido

# port = mido.open_output('Port Name')

mid = mido.MidiFile('Tools/test1.mid')
for i in mid.play():
    print(i)