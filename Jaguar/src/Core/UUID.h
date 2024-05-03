#pragma once


#include <xhash>

namespace Jaguar
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& uuid) = default;
		~UUID();
	
		operator uint64_t() const { return m_UUID; }
	
	private:
		uint64_t m_UUID;
	};
	
}

namespace std
{	template<>
	struct hash<Jaguar::UUID>
	{
	std::size_t operator()(const Jaguar::UUID& uuid)
	{
		return hash<uint64_t>()((uint64_t)uuid);
	}
	};
}