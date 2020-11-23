#!ruby

=begin
Reference:
  * https://docs.ruby-lang.org/ja/master/library/benchmark.html
=end

module Benchmark
  def Benchmark.benchmark(caption = "", label_width = nil, fmtstr = nil, *labels) #{|rep| ...} -> bool
  end

  def Benchmark.bm(label_width = 0, *labels) #{|rep| ... } -> bool
    rep = Reporter
  end

  def Benchmark.bmbm(width = 0) #{|job| ... } -> [Benchmark::Tms]
  end

  def Benchmark.measure(label = "", &block)
    a = __measure_start
    yield
    b = __measure_finish
    d = [
      a[0].diff_usec(b[0]),
      a[1].diff_usec(b[1]),
      a[2].diff_usec(b[2]),
      a[3].diff_usec(b[3]),
      a[4].diff_usec(b[4]),
    ]
    Tms.new(*d, d[0] + d[1] + d[2] + d[3], label)
  end

  def Benchmark.realtime(&b)
    _realtime_us(&b) / 1000000.0
  end

  # Benchmark.realtime とは異なり、経過時間は micro second (us) で Integer として返します。
  # MRB_NO_FLOAT (MRB_WITHOUT_FLOAT) が定義された環境向けのメソッドです。
  public
  def Benchmark._realtime_us
    st = __measure_realtime
    yield
    __measure_realtime - st
  end

  # CRuby とは異なり、経過時間は micro second (us) で Integer として保存します。
  class Tms < Struct.new(:utime, :stime, :cutime, :cstime, :real, :total, :label)
    BasicStruct = superclass
  end
end
