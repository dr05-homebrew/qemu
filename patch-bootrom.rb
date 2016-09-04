#!/usr/bin/ruby

require 'digest'

if ARGV.length != 1 && ARGV.length != 2
    puts "Usage: ./patch-bootrom.rb INPUT [OUTPUT]"
end

input = ARGV[0]
output = ARGV[1] || "#{input}.patched"

d = File.read input, :encoding => 'binary'

good_hashes = [
    "5479b92e333c79735ba44306b477a62f865aa124"
]

hash = Digest::SHA1.hexdigest d
if good_hashes.include? hash
    puts "Good SHA1 sum #{hash}"
else
    puts "Warning: Unknown SHA1 sum #{hash}"
end

ent = 0xef000000
puts "Patching the entry point to 0x#{ent.to_s 16}"
d[0x18, 4] = [ent].pack("V")

puts "Avoiding otp_read"

# nop at ef00085a
d[0x5cd8, 4] = [0, 0, 0, 0].pack('C*')
d[0x5ce4, 4] = [0, 0, 0, 0].pack('C*')

# nop at ef0008ce
d[0x5d4c, 4] = [0, 0, 0, 0].pack('C*')
d[0x5d58, 4] = [0, 0, 0, 0].pack('C*')

# nop at ef0008e6
d[0x5d64, 4] = [0, 0, 0, 0].pack('C*')
d[0x5d74, 4] = [0, 0, 0, 0].pack('C*')
d[0x5d88, 4] = [0, 0, 0, 0].pack('C*')

# make the jump at ef000906 unconditional
d[0x5d7c, 2] = [0x19, 0x20].pack('C*')

# nop out calls to syscontrol at ef0008a0
d[0x5d1c, 4] = [0, 0, 0, 0].pack('C*')
d[0x5d22, 4] = [0, 0, 0, 0].pack('C*')


puts "Allowing boot modes"

# _bootrom.preboot.bootmodes: R7 = 0; there are no disabled bootmodes
d[0x5dae, 2] = [0x07, 0x60].pack('C*')

# ... and don't do BITSET(R7, 0)!
d[0x5db0, 2] = [0x00, 0x00].pack('C*')

# make the jump at _bootrom.preboot.bootmodes.end unconditional
d[0x5dcc, 2] = [0x26, 0x20].pack('C*')

puts "Writing patched file to #{output}"
File.write output, d
