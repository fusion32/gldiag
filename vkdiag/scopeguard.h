#ifndef SCOPEGUARD_H_
#define SCOPEGUARD_H_

template<typename F>
class ScopeGuard{
private:
	F func;
	bool active;
public:
	ScopeGuard(F &&func_) : func(std::forward<F>(func_)), active(true) {}
	~ScopeGuard(void) { if(active) func(); }
	void dismiss(void) { active = false; }
};

template<typename F>
ScopeGuard<F> make_scope_guard(F &&func){
	return ScopeGuard<F>(std::forward<F>(func));
}

#define CONCATENATE_IMPL(a, b) a##b
#define CONCATENATE(a, b) CONCATENATE_IMPL(a, b)

#define SCOPE_EXIT(f)					\
	auto CONCATENATE(ON_SCOPE_EXIT, __COUNTER__) =	\
		make_scope_guard(f)

#endif //SCOPEGUARD_H_
