#!ruby

require "yaml"

config = YAML.load <<'YAML'
  common:
    gems:
    - :core: "mruby-sprintf"
    - :core: "mruby-print"
    - :core: "mruby-bin-mrbc"
    - :core: "mruby-bin-mirb"
    - :core: "mruby-bin-mruby"
  builds:
    host:
      gems:
      - :core: "mruby-io"
    host++-word:
      defines: MRB_WORD_BOXING
      c++abi: true
YAML

config["builds"].each_pair do |n, c|
  MRuby::Build.new(n) do |conf|
    toolchain :clang

    conf.build_dir = File.join("build", c["build_dir"] || name)

    enable_debug
    enable_test
    enable_cxx_abi if c["c++abi"]

    cc.defines << [*c["defines"]]
    cc.flags << [*c["cflags"]]

    Array(config.dig("common", "gems")).each { |*g| gem *g rescue nil }
    Array(c["gems"]).each { |*g| gem *g rescue nil }

    gem __dir__ do |g|
      if g.cc.command =~ /\b(?:g?cc|clang)\d*\b/
        g.cc.flags << "-std=c11" unless c["c++abi"]
        g.cc.flags << "-pedantic"
        g.cc.flags << "-Wall"
      end
    end
  end
end
