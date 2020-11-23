#!ruby

begin
  require "mruby/source"
rescue LoadError
  $: << File.expand_path(File.join(MRUBY_ROOT, "lib"))
  require "mruby/source"
end

if MRuby::Source::MRUBY_RELEASE_NO < 10300
  fail %(  ### ## # "mruby-benchmark" needs mruby-1.3.0 or later # ## ###)
end

MRuby::Gem::Specification.new("mruby-benchmark") do |s|
  s.summary = "CRuby の Benchmark のようなもの"
  version = File.read(File.join(__dir__, "README.md")).scan(/^\s*[\-\*] version:\s*(\d+(?:\.\w+)+)/i).flatten[-1]
  s.version = version if version
  s.license = "BSD-2-Clause"
  s.author  = "dearblue"
  s.homepage = "https://github.com/dearblue/mruby-benchmark"

  add_dependency "mruby-error", core: "mruby-error"
  add_dependency "mruby-string-ext", core: "mruby-string-ext"
  add_dependency "mruby-struct", core: "mruby-struct"

  if cc.command =~ /\b(?:g?cc|clang)\d*\b/
    cc.flags << %w(-Wno-declaration-after-statement)
  end
end
