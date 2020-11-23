#include <mruby.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/error.h>
#include <mruby/istruct.h>
#include <mruby/version.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#define CLASSNAME_BENCHMARK "Benchmark"
#define CLASSNAME_BMTIME "Time__"

#if MRUBY_RELEASE_NO < 20000
# define RIStruct RIstruct
#endif

static struct RClass *
ext_get_bmtime_class(mrb_state *mrb)
{
  struct RClass *bmtime_class = mrb_class_get_under(mrb, mrb_module_get(mrb, CLASSNAME_BENCHMARK), CLASSNAME_BMTIME);

  if (MRB_INSTANCE_TT(bmtime_class) != MRB_TT_ISTRUCT) {
    mrb_raise(mrb, E_TYPE_ERROR, "type mismatch for " CLASSNAME_BENCHMARK "::" CLASSNAME_BMTIME);
  }

  return bmtime_class;
}

static struct RIStruct *
ext_alloc_measure_bmtime(mrb_state *mrb, struct RClass *clss)
{
  if (clss == NULL) {
    clss = ext_get_bmtime_class(mrb);
  }

  return (struct RIStruct *)mrb_obj_alloc(mrb, MRB_TT_ISTRUCT, clss);
}

/*
 * - 日付を扱うわけではないので、64ビット time_t の下位32ビットを保存するだけで事足りる
 * - 32ビット環境で64ビット整数型を扱う場合、アラインメントを気にする必要がある
 */
struct measure_bmtime
{
  int32_t sec;
  int32_t usec;
};

/*
 * @return `[utime, stime, cutime, cstime, real]`
 */
static mrb_value
benchmark_s_internals_measure_start(mrb_state *mrb, mrb_value mod)
{
  /* オブジェクト生成のオーバーヘッドを最小化させる */

  struct RClass *bmtime_class = mrb_class_get_under(mrb, mrb_class_ptr(mod), CLASSNAME_BMTIME);
  struct {
    struct RIStruct *utime, *stime, *cutime, *cstime, *real;
  } times = {
    ext_alloc_measure_bmtime(mrb, bmtime_class),
    ext_alloc_measure_bmtime(mrb, bmtime_class),
    ext_alloc_measure_bmtime(mrb, bmtime_class),
    ext_alloc_measure_bmtime(mrb, bmtime_class),
    ext_alloc_measure_bmtime(mrb, bmtime_class),
  };
  mrb_value retval = mrb_ary_new_capa(mrb, sizeof(times) / sizeof(times.utime));
  mrb_ary_set(mrb, retval, 0, mrb_obj_value(times.utime));
  mrb_ary_set(mrb, retval, 1, mrb_obj_value(times.stime));
  mrb_ary_set(mrb, retval, 2, mrb_obj_value(times.cutime));
  mrb_ary_set(mrb, retval, 3, mrb_obj_value(times.cstime));
  mrb_ary_set(mrb, retval, 4, mrb_obj_value(times.real));

  struct rusage itself;
  struct rusage children;
  struct timespec real;

  getrusage(RUSAGE_SELF, &itself);
  getrusage(RUSAGE_CHILDREN, &children);
  clock_gettime(CLOCK_MONOTONIC, &real);

  ((struct measure_bmtime *)times.utime->inline_data)->sec = itself.ru_utime.tv_sec;
  ((struct measure_bmtime *)times.utime->inline_data)->usec = itself.ru_utime.tv_usec;
  ((struct measure_bmtime *)times.stime->inline_data)->sec = itself.ru_stime.tv_sec;
  ((struct measure_bmtime *)times.stime->inline_data)->usec = itself.ru_stime.tv_usec;
  ((struct measure_bmtime *)times.cutime->inline_data)->sec = children.ru_utime.tv_sec;
  ((struct measure_bmtime *)times.cutime->inline_data)->usec = children.ru_utime.tv_usec;
  ((struct measure_bmtime *)times.cstime->inline_data)->sec = children.ru_stime.tv_sec;
  ((struct measure_bmtime *)times.cstime->inline_data)->usec = children.ru_stime.tv_usec;
  ((struct measure_bmtime *)times.real->inline_data)->sec = real.tv_sec;
  ((struct measure_bmtime *)times.real->inline_data)->usec = real.tv_nsec / 1000;

  return retval;
}

static mrb_value
benchmark_s_internals_measure_finish(mrb_state *mrb, mrb_value mod)
{
  struct rusage itself;
  struct rusage children;
  struct timespec real;

  getrusage(RUSAGE_SELF, &itself);
  getrusage(RUSAGE_CHILDREN, &children);
  clock_gettime(CLOCK_MONOTONIC, &real);

  struct RClass *bmtime_class = mrb_class_get_under(mrb, mrb_class_ptr(mod), CLASSNAME_BMTIME);
  struct {
    struct RIStruct *utime, *stime, *cutime, *cstime, *real;
  } times = {
    ext_alloc_measure_bmtime(mrb, bmtime_class),
    ext_alloc_measure_bmtime(mrb, bmtime_class),
    ext_alloc_measure_bmtime(mrb, bmtime_class),
    ext_alloc_measure_bmtime(mrb, bmtime_class),
    ext_alloc_measure_bmtime(mrb, bmtime_class),
  };
  mrb_value retval = mrb_ary_new_capa(mrb, sizeof(times) / sizeof(times.utime));
  mrb_ary_set(mrb, retval, 0, mrb_obj_value(times.utime));
  mrb_ary_set(mrb, retval, 1, mrb_obj_value(times.stime));
  mrb_ary_set(mrb, retval, 2, mrb_obj_value(times.cutime));
  mrb_ary_set(mrb, retval, 3, mrb_obj_value(times.cstime));
  mrb_ary_set(mrb, retval, 4, mrb_obj_value(times.real));

  ((struct measure_bmtime *)times.utime->inline_data)->sec = itself.ru_utime.tv_sec;
  ((struct measure_bmtime *)times.utime->inline_data)->usec = itself.ru_utime.tv_usec;
  ((struct measure_bmtime *)times.stime->inline_data)->sec = itself.ru_stime.tv_sec;
  ((struct measure_bmtime *)times.stime->inline_data)->usec = itself.ru_stime.tv_usec;
  ((struct measure_bmtime *)times.cutime->inline_data)->sec = children.ru_utime.tv_sec;
  ((struct measure_bmtime *)times.cutime->inline_data)->usec = children.ru_utime.tv_usec;
  ((struct measure_bmtime *)times.cstime->inline_data)->sec = children.ru_stime.tv_sec;
  ((struct measure_bmtime *)times.cstime->inline_data)->usec = children.ru_stime.tv_usec;
  ((struct measure_bmtime *)times.real->inline_data)->sec = real.tv_sec;
  ((struct measure_bmtime *)times.real->inline_data)->usec = real.tv_nsec / 1000;

  return retval;
}

static mrb_value
benchmark_s_internals_measure_realtime(mrb_state *mrb, mrb_value mod)
{
  struct timespec real;

  clock_gettime(CLOCK_MONOTONIC, &real);

  return mrb_fixnum_value(real.tv_sec * 1000000 + real.tv_nsec / 1000);
}

static mrb_value
bmtime_diff_usec(mrb_state *mrb, mrb_value self)
{
  mrb_value other;
  mrb_get_args(mrb, "o", &other);
  struct RClass *bmtime_class = ext_get_bmtime_class(mrb);
  if (mrb_type(other) != MRB_TT_ISTRUCT || !mrb_obj_is_kind_of(mrb, other, bmtime_class)) {
    mrb_raise(mrb, E_TYPE_ERROR, "not a " CLASSNAME_BENCHMARK "::" CLASSNAME_BMTIME);
  }

  struct measure_bmtime *ap = (struct measure_bmtime *)mrb_istruct_ptr(self);
  struct measure_bmtime *bp = (struct measure_bmtime *)mrb_istruct_ptr(other);
  mrb_int usec = (bp->sec - ap->sec) * 1000000 + (bp->usec - ap->usec);

  return mrb_fixnum_value(usec);
}

void
mrb_mruby_benchmark_gem_init(mrb_state *mrb)
{
  struct RClass *benchmark_module = mrb_define_module(mrb, CLASSNAME_BENCHMARK);
  mrb_define_class_method(mrb, benchmark_module, "__measure_start", benchmark_s_internals_measure_start, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, benchmark_module, "__measure_finish", benchmark_s_internals_measure_finish, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, benchmark_module, "__measure_realtime", benchmark_s_internals_measure_realtime, MRB_ARGS_NONE());

  struct RClass *bmtime_class = mrb_define_class_under(mrb, benchmark_module, CLASSNAME_BMTIME, mrb->object_class);
  MRB_SET_INSTANCE_TT(bmtime_class, MRB_TT_ISTRUCT);
  mrb_define_method(mrb, bmtime_class, "diff_usec", bmtime_diff_usec, MRB_ARGS_REQ(1));
}

void
mrb_mruby_benchmark_gem_final(mrb_state *mrb)
{
}
